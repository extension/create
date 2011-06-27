<?php
// $Id: kaltura.admin.inc,v 1.1.2.5.2.10 2011/01/27 12:14:00 xuriz Exp $

/*
 * @file
 * contains functions for administration use of the kaltura core module
 *
 */

/*
 * helper function that process the partner registration with kaltura
 *
 * if registration successful, the partner details are saved as system variables (drupal DB) and
 * the user is being redirected to the modules settings page where he should see the details inserted
 * and a configuration test which says that the configuraion works
 * TODO set up for new API
 * CMAC
 * changes:
 * removed references to categories
 * questions: this method created problems within getServiceConfiguration wrt the Logger
 */
function kaltura_register_partner($values) {
  global $base_url;
  //get module version
  $infopath = drupal_get_path('module', 'kaltura') . '/' . 'kaltura' . '.info';
  $info = drupal_parse_info_file($infopath);
  $module_version = !empty($info['version']) ? $info['version'] : 'N/A';

  $partner_name = ($values['server_url'] == 'http://www.kaltura.com') ? NULL : $values['first_name'] . '_' . $values['last_name'];
  $cms_pass = kaltura_generate_cms_password();
  //maybe mark fields them in the form
  $addit_val_array = array('title', 'vertical', 'would_you_like_to_be_contacted');
  $additional = array();
  foreach ($addit_val_array as $val) {
   $parm = new KalturaKeyValue();
   $parm->key = $val;
   $parm->value = $values[$val];
   $additional[] = $parm;
  }

  $partner = new KalturaPartner();
  //what is this field?
  $partner->name =             $values['company'];
  $partner->phone =            $values['phone'];
  $partner->website =          $values['website'];
  $partner->notificationUrl =   $values['kaltura_partner_url2'] .'/?q=kaltura/notification_handler';
  if (!empty($partner_name)) {
    $partner->adminName =         $partner_name;
    $partner->firstName =         NULL;
    $partner->lastName =          NULL;
  }
  else {
    $partner->firstName =         $values['first_name'];
    $partner->lastName =          $values['last_name'];
  }
  $partner->adminEmail =        $values['email'];
  $partner->country =           $values['country'];
  $partner->state =             $values['state'];
  $partner->description =       $values['k_description'] . '|DRUPAL' . VERSION . '|module ver ' . $module_version;
  $partner->commercialUse =     "non-commercial_use";
  $partner->type =              102;
  $partner->appearInSearch =    '1';
  $partner->additionalParams =  $additional;
  $session_user = KalturaHelpers::getSessionUser();
  $config = KalturaHelpers::getServiceConfiguration();
  $config->serviceUrl = $values['server_url'];
  $kaltura_client = new KalturaClient($config);
//  $kaltura_client->setConfig($config);

  watchdog('regis', print_r($partner, TRUE));
  try {
    $res = $kaltura_client->partner->register($partner, $cms_pass);
    $res->cms_pass = $cms_pass;
  }
  catch (Exception $e) {
    $res = $e;
  }
  return $res;
}


/*
 * helper function to retrieve partner's details from kaltura
 * TODO: testing // who is using this one?
 * CMAC changes:
 * kaltura client config set up to reflect changes in the API
 * getsecrets now called instead of getPartner
 * return array built using Kaltura Partner object
 *
 */
function kaltura_get_partner_details($partner_id, $cms_email, $cms_password, $server_url) {

  $session_user = KalturaHelpers::getSessionUser();
  $config = KalturaHelpers::getServiceConfiguration();
  $config->serviceUrl = $server_url;
  $config->partnerId = $partner_id;
  $kaltura_client = new KalturaClient($config);
  /**
   * here we are trying to retrieve the partner details from the kaltura server
   *
   */

  try {
    $result = $kaltura_client->partner->getsecrets($partner_id, $cms_email, $cms_password);
  }
  catch (Exception $e) {
    $result = $e;
  }
  return $result;
}

/*
 * helper function that uses Kaltura's getDefaultWidget service to create some default widgets for
 * the partner, in case he does not have these yet
 * TODO
 * CMAC
 * changes
 * changed client initialisation
 * changed the getDefaultWidget function call to $kaltura_client->widget->get
 * question: what do we do with the widget when we get them?
 */
function kaltura_create_widgets_for_partner($partner_id, $secret) {
  /*$session_user = KalturaHelpers::getSessionUser();
   $config = KalturaHelpers::getServiceConfiguration();
   $config->partnerId = $partner_id;
   $kaltura_client = new KalturaClient();
   $kaltura_client->setConfig($config);*/

  $session_user = KalturaHelpers::getSessionUser();
  $kaltura_client = KalturaHelpers::getKalturaClient(1);

  $widgets = new KalturaSettings;

  foreach ($widgets->kdp_widgets as $type => $uiconfs) {
    foreach ($uiconfs as $skin => $confs) {
    /* oferc: that doesn't work for me yet */
//      $result = $kaltura_client->widget->get("_1_".$confs['view_uiconf']);
//      $result = $kaltura_client->widget->get("_1_".$confs['remix_uiconf']);
    }
  }
}

/*
 * determine how the general settings form will look like.
 *
 * returns one of 2 states of forms:
 * 1) registration form (if variables do not exist in the DB)
 *    - either as SaaS or Kaltura CE
 * 2) notifications settings + configuration test + settings form
 *
 */

function kaltura_registration_form() {

  module_load_include('php', 'kaltura', 'kaltura_client/KalturaClient');
  drupal_add_css(drupal_get_path("module", "kaltura") . "/style/kaltura_reg_form.css");
  drupal_add_js(drupal_get_path("module", "kaltura") . "/js/jquery.validate.min.js", array('group' => JS_LIBRARY));
  drupal_add_js(drupal_get_path("module", "kaltura") . "/js/additional-methods.js", array('group' => JS_LIBRARY));
  global $base_url;
  $form = array();
  //check if we are all ready registerd if so show status page and partner info
  if (variable_get('kaltura_partner_id', '') != '') {
    $form = kaltura_status_form();
    return $form;
  }
  $form['first_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['first_set']['server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get('kaltura_server_url', 'http://www.kaltura.com'),
    '#description' => t('If you are working against Kaltura SaaS, leave this setting as it is.</br> Otherwise, if you are working against a Kaltura CE or Kaltura On-Prem server, type the server URL here.'),
  );
  $form['first_set']['kaltura_partner_url2'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification URL'),
    '#default_value' => variable_get('kaltura_partner_url2', $base_url),
    '#description' => t('server notifications allows the Kaltura video platform to update your Drupal server about the status of the media content. This is especially useful when uploading content in various formats that needs to be transcoded. In order to support server side notifications, your server must be publicly available on the internet. The notification URL is where notifications are sent to and where a notification handler script resides.'),
  );
  $form['first_set']['new_partner'] = array(
    '#type' => 'radios',
    '#title' => t('Partner'),
    '#options' => array(
      'new' => t('New'),
      'existing' => t('Existing'),
    ),
    '#default_value' => t('new'),
  );
  $form['ex_partner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Existing partner registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="new_partner"]' => array('value' => t('existing')),
      )
    )
  );
  $form['ex_partner']['ex_partner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner ID'),
    '#size' => 10,
  );
  $form['ex_partner']['ex_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
  );
  $form['ex_partner']['ex_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#maxlength' => 64,
  );
  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('New partner registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="new_partner"]' => array('value' => t('new')),
      )
    )
  );
  $form['registration']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website'),
    '#size' => 30,
  );
  $form['registration']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Job Title'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['vertical'] = array(
    '#type' => 'select',
    '#title' => t('Describe yourself'),
    //'#description' => t('we may need this'),
    '#options' => array(
      'Enterprise' => t('Enterprise / Small Business / Government Agency'),
      'Education' => t('Education Organization'),
      'Media' => t('Media Company / Agency'),
      'Service Provider' => t('CDN / ISP / Integrator / Hosting'),
      'Other' => t('Other'),
    ),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    //'#description' => t('/* description */'),
    '#options' => kaltura_contry_opts(),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );
  $form['registration']['state'] = array(
    '#type' => 'select',
    '#title' => t('State / Province'),
    //'#description' => t('/* description */'),
    '#options' => kaltura_us_states(),
    '#states' => array(
      'visible' => array(
        ':input[name="country"]' => array('value' => 'US'),
      )
    ),
    '#default_value' => 'Not Applicable',
  );
  $form['registration']['would_you_like_to_be_contacted'] = array(
    '#type' => 'select',
    '#title' => t('Would you like a Kaltura Video Expert to contact you?'),
    //'#description' => t('/* description */'),
    '#options' => array(
      'yes' => t('Yes'),
      'no'  => t('Not right now'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="server_url"]' => array('value' => 'http://www.kaltura.com'),
      )
    ),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
    '#default_value' => 'yes',
  );
  $form['registration']['k_description'] = array(
    '#type' => 'textarea',
    '#title' => t('How do you plan to use Kaltura\'s video platform?'),
    '#cols' => 60,
  );
  $form['terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('I accept ' . l('The Kaltura service Terms of Use', 'http://corp.kaltura.com/terms_of_use_drupal')),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );
  return $form;
}




function kaltura_registration_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['new_partner'] == 'new') {
    $required = array('first_name', 'last_name', 'email', 'phone', 'company', 'title', 'country', 'would_you_like_to_be_contacted');
    foreach ($required as $field) {
      if (empty($values[$field])) {
        form_set_error('', t('the field ' . str_replace('_', ' ', $field) . ' is required'));
      }
    }
  }
  else {
    $required = array('ex_partner_id', 'ex_email', 'ex_password');
    foreach ($required as $field) {
      if (empty($values[$field])) {
        form_set_error('', t('the field ' . str_replace('ex_', ' ', $field) . ' is required'));
      }
    }
  }
}


function kaltura_registration_form_submit($from, $form_state) {
  $values = $form_state['values'];
  if ($values['new_partner'] == 'existing') {
    $res = kaltura_get_partner_details($values['ex_partner_id'], $values['ex_email'], $values['ex_password'], $values['server_url']);
  }
  else {
    $res = kaltura_register_partner($values);
  }
  //dsm($res['error']);
  $result = $res->id;
  if ($result) {
    variable_set('kaltura_partner_id',         $res->id);
    variable_set('kaltura_subp_id',            $res->id."00");
    variable_set('kaltura_secret',             $res->secret);
    variable_set('kaltura_admin_secret',       $res->adminSecret);
    variable_set('kaltura_local_registration', 1);
    variable_set('kaltura_server_url', $values['server_url']);
    variable_set('kaltura_partner_cms_email', $res->adminEmail);
    variable_set('kaltura_partner_cms_password', $res->cmsPassword);
    variable_set('kaltura_partner_url2', $values['kaltura_partner_url2']);
    variable_set('kaltura_notification_type', 1);
    kaltura_update_kaltura_partner(2, $res->notificationUrl);
    kaltura_create_widgets_for_partner($res->id, $res->secret);
    drupal_set_message('Congratulations!  You have successfully installed the All in One Video Module and registered for the Kaltura Partner Program.');
    drupal_goto('admin/config/media/kaltura');
  }
  else {
    $code = $res->getCode();
    $msg = $res->getMessage();
    if ($code == 'PARTNER_REGISTRATION_ERROR') {
      $msg = t('It seems you already have an account with Kaltura (the email you provided is in our records). If you want to create another Kaltura account, please enter a different email address.');
    }
    drupal_set_message($msg, 'error');
  }
}

/**
 * kaltura_contry_opts
 * helper function that returns a list of countrys
 * @access public
 * @return void
 */
function kaltura_contry_opts() {
  $opts = array(
            'AF' =>    'Afganistan',
            'AL' =>    'Albania',
            'DZ' =>    'Algeria',
            'AS' => 'American Samoa',
            'AD' => 'Andorra',
            'AO' => 'Angola',
            'AI' => 'Anguilla',
            'AQ' => 'Antarctica',
            'AG' => 'Antigua and Barbuda',
            'AR' => 'Argentina',
            'AM' => 'Armenia',
            'AW' => 'Aruba',
            'AU' => 'Australia',
            'AT' => 'Austria',
            'AZ' => 'Azerbaijan',
            'BS' => 'Bahamas',
            'BH' => 'Bahrain',
            'BD' => 'Bangladesh',
            'BB' => 'Barbados',
            'BY' => 'Belarus',
            'BE' => 'Belgium',
            'BZ' => 'Belize',
            'BJ' => 'Benin',
            'BM' => 'Bermuda',
            'BT' => 'Bhutan',
            'BO' => 'Bolivia',
            'BA' => 'Bosnia and Herzegowina',
            'BW' => 'Botswana',
            'BV' => 'Bouvet Island',
            'BR' => 'Brazil',
            'IO' => 'British Indian Ocean Territory',
            'BN' => 'Brunei Darussalam',
            'BG' => 'Bulgaria',
            'BF' => 'Burkina Faso',
            'BI' => 'Burundi',
            'KH' => 'Cambodia',
            'CM' => 'Cameroon',
            'CA' => 'Canada',
            'CV' => 'Cape Verde',
            'KY' => 'Cayman Islands',
            'CF' => 'Central African Republic',
            'TD' => 'Chad',
            'CL' => 'Chile',
            'CN' => 'China',
            'CX' => 'Christmas Island',
            'CC' => 'Cocos (Keeling) Islands',
            'CO' => 'Colombia',
            'KM' => 'Comoros',
            'CG' => 'Congo',
            'CD' => 'Congo, the Democratic Republic of the',
            'CK' => 'Cook Islands',
            'CR' => 'Costa Rica',
            'CI' => 'Cote d\'Ivoire',
            'HR' => 'Croatia (Hrvatska)',
            'CU' => 'Cuba',
            'CY' => 'Cyprus',
            'CZ' => 'Czech Republic',
            'DK' => 'Denmark',
            'DJ' => 'Djibouti',
            'DM' => 'Dominica',
            'DO' => 'Dominican Republic',
            'TP' => 'East Timor',
            'EC' => 'Ecuador',
            'EG' => 'Egypt',
            'SV' => 'El Salvador',
            'GQ' => 'Equatorial Guinea',
            'ER' => 'Eritrea',
            'EE' => 'Estonia',
            'ET' => 'Ethiopia',
            'FK' => 'Falkland Islands (Malvinas)',
            'FO' => 'Faroe Islands',
            'FJ' => 'Fiji',
            'FI' => 'Finland',
            'FR' => 'France',
            'FX' => 'France, Metropolitan',
            'GF' => 'French Guiana',
            'PF' => 'French Polynesia',
            'TF' => 'French Southern Territories',
            'GA' => 'Gabon',
            'GM' => 'Gambia',
            'GE' => 'Georgia',
            'DE' => 'Germany',
            'GH' => 'Ghana',
            'GI' => 'Gibraltar',
            'GR' => 'Greece',
            'GL' => 'Greenland',
            'GD' => 'Grenada',
            'GP' => 'Guadeloupe',
            'GU' => 'Guam',
            'GT' => 'Guatemala',
            'GN' => 'Guinea',
            'GW' => 'Guinea-Bissau',
            'GY' => 'Guyana',
            'HT' => 'Haiti',
            'HM' => 'Heard and Mc Donald Islands',
            'VA' => 'Holy See (Vatican City State)',
            'HN' => 'Honduras',
            'HK' => 'Hong Kong',
            'HU' => 'Hungary',
            'IS' => 'Iceland',
            'IN' => 'India',
            'ID' => 'Indonesia',
            'IR' => 'Iran (Islamic Republic of)',
            'IQ' => 'Iraq',
            'IE' => 'Ireland',
            'IL' => 'Israel',
            'IT' => 'Italy',
            'JM' => 'Jamaica',
            'JP' => 'Japan',
            'JO' => 'Jordan',
            'KZ' => 'Kazakhstan',
            'KE' => 'Kenya',
            'KI' => 'Kiribati',
            'KP' => 'Korea, Democratic People\'s Republic of',
            'KR' => 'Korea, Republic of',
            'KW' => 'Kuwait',
            'KG' => 'Kyrgyzstan',
            'LA' => 'Lao People\'s Democratic Republic',
            'LV' => 'Latvia',
            'LB' => 'Lebanon',
            'LS' => 'Lesotho',
            'LR' => 'Liberia',
            'LY' => 'Libyan Arab Jamahiriya',
            'LI' => 'Liechtenstein',
            'LT' => 'Lithuania',
            'LU' => 'Luxembourg',
            'MO' => 'Macau',
            'MK' => 'Macedonia, The Former Yugoslav Republic of',
            'MG' => 'Madagascar',
            'MW' => 'Malawi',
            'MY' => 'Malaysia',
            'MV' => 'Maldives',
            'ML' => 'Mali',
            'MT' => 'Malta',
            'MH' => 'Marshall Islands',
            'MQ' => 'Martinique',
            'MR' => 'Mauritania',
            'MU' => 'Mauritius',
            'YT' => 'Mayotte',
            'MX' => 'Mexico',
            'FM' => 'Micronesia, Federated States of',
            'MD' => 'Moldova, Republic of',
            'MC' => 'Monaco',
            'MN' => 'Mongolia',
            'MS' => 'Montserrat',
            'MA' => 'Morocco',
            'MZ' => 'Mozambique',
            'MM' => 'Myanmar',
            'NA' => 'Namibia',
            'NR' => 'Nauru',
            'NP' => 'Nepal',
            'NL' => 'Netherlands',
            'AN' => 'Netherlands Antilles',
            'NC' => 'New Caledonia',
            'NZ' => 'New Zealand',
            'NI' => 'Nicaragua',
            'NE' => 'Niger',
            'NG' => 'Nigeria',
            'NU' => 'Niue',
            'NF' => 'Norfolk Island',
            'MP' => 'Northern Mariana Islands',
            'NO' => 'Norway',
            'OM' => 'Oman',
            'PK' => 'Pakistan',
            'PW' => 'Palau',
            'PA' => 'Panama',
            'PG' => 'Papua New Guinea',
            'PY' => 'Paraguay',
            'PE' => 'Peru',
            'PH' => 'Philippines',
            'PN' => 'Pitcairn',
            'PL' => 'Poland',
            'PT' => 'Portugal',
            'PR' => 'Puerto Rico',
            'QA' => 'Qatar',
            'RE' => 'Reunion',
            'RO' => 'Romania',
            'RU' => 'Russian Federation',
            'RW' => 'Rwanda',
            'KN' => 'Saint Kitts and Nevis',
            'LC' => 'Saint LUCIA',
            'VC' => 'Saint Vincent and the Grenadines',
            'WS' => 'Samoa',
            'SM' => 'San Marino',
            'ST' => 'Sao Tome and Principe',
            'SA' => 'Saudi Arabia',
            'SN' => 'Senegal',
            'SC' => 'Seychelles',
            'SL' => 'Sierra Leone',
            'SG' => 'Singapore',
            'SK' => 'Slovakia (Slovak Republic)',
            'SI' => 'Slovenia',
            'SB' => 'Solomon Islands',
            'SO' => 'Somalia',
            'ZA' => 'South Africa',
            'GS' => 'South Georgia and the South Sandwich Islands',
            'ES' => 'Spain',
            'LK' => 'Sri Lanka',
            'SH' => 'St. Helena',
            'PM' => 'St. Pierre and Miquelon',
            'SD' => 'Sudan',
            'SR' => 'Suriname',
            'SJ' => 'Svalbard and Jan Mayen Islands',
            'SZ' => 'Swaziland',
            'SE' => 'Sweden',
            'CH' => 'Switzerland',
            'SY' => 'Syrian Arab Republic',
            'TW' => 'Taiwan, Province of China',
            'TJ' => 'Tajikistan',
            'TZ' => 'Tanzania, United Republic of',
            'TH' => 'Thailand',
            'TG' => 'Togo',
            'TK' => 'Tokelau',
            'TO' => 'Tonga',
            'TT' => 'Trinidad and Tobago',
            'TN' => 'Tunisia',
            'TR' => 'Turkey',
            'TM' => 'Turkmenistan',
            'TC' => 'Turks and Caicos Islands',
            'TV' => 'Tuvalu',
            'UG' => 'Uganda',
            'UA' => 'Ukraine',
            'AE' => 'United Arab Emirates',
            'GB' => 'United Kingdom',
            'US' => 'United States',
            'UM' => 'United States Minor Outlying Islands',
            'UY' => 'Uruguay',
            'UZ' => 'Uzbekistan',
            'VU' => 'Vanuatu',
            'VE' => 'Venezuela',
            'VN' => 'Viet Nam',
            'VG' => 'Virgin Islands (British)',
            'VI' => 'Virgin Islands (U.S.)',
            'WF' => 'Wallis and Futuna Islands',
            'EH' => 'Western Sahara',
            'YE' => 'Yemen',
            'YU' => 'Yugoslavia',
            'ZM' => 'Zambia',
            'ZW' => 'Zimbabwe'
          );
  return $opts;

}

/**
 * kaltura_us_states
 * helper function that returns a list of US states
 * @access public
 * @return void
 */
function kaltura_us_states() {
  $state_list = array(
      'NA' => 'Not Applicable',
      'AL'=>"Alabama",
      'AK'=>"Alaska",
      'AZ'=>"Arizona",
      'AR'=>"Arkansas",
      'CA'=>"California",
      'CO'=>"Colorado",
      'CT'=>"Connecticut",
      'DE'=>"Delaware",
      'DC'=>"District Of Columbia",
      'FL'=>"Florida",
      'GA'=>"Georgia",
      'HI'=>"Hawaii",
      'ID'=>"Idaho",
      'IL'=>"Illinois",
      'IN'=>"Indiana",
      'IA'=>"Iowa",
      'KS'=>"Kansas",
      'KY'=>"Kentucky",
      'LA'=>"Louisiana",
      'ME'=>"Maine",
      'MD'=>"Maryland",
      'MA'=>"Massachusetts",
      'MI'=>"Michigan",
      'MN'=>"Minnesota",
      'MS'=>"Mississippi",
      'MO'=>"Missouri",
      'MT'=>"Montana",
      'NE'=>"Nebraska",
      'NV'=>"Nevada",
      'NH'=>"New Hampshire",
      'NJ'=>"New Jersey",
      'NM'=>"New Mexico",
      'NY'=>"New York",
      'NC'=>"North Carolina",
      'ND'=>"North Dakota",
      'OH'=>"Ohio",
      'OK'=>"Oklahoma",
      'OR'=>"Oregon",
      'PA'=>"Pennsylvania",
      'RI'=>"Rhode Island",
      'SC'=>"South Carolina",
      'SD'=>"South Dakota",
      'TN'=>"Tennessee",
      'TX'=>"Texas",
      'UT'=>"Utah",
      'VT'=>"Vermont",
      'VA'=>"Virginia",
      'WA'=>"Washington",
      'WV'=>"West Virginia",
      'WI'=>"Wisconsin",
      'WY'=>"Wyoming"
    );
  return $state_list;

}

function kaltura_status_form() {
  global $base_url;
  $form['kaltura_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kaltura Notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['kaltura_notifications']['kaltura_partner_url2'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification URL'),
    '#default_value' => variable_get('kaltura_partner_url2', $base_url),
    '#disabled' => TRUE,
    '#description' => t('Server notifications allows the Kaltura video platform to update your Drupal server about the status of the media content. This is especially useful when uploading content in various formats that needs to be transcoded. In order to support server side notifications, your server must be publicly available on the internet.'),
  );
  $form['kaltura_server_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server Status'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  list($admin_session_status, $admin_session_error) = kaltura_check_server_status($admin = 1);
  list($normal_session_status, $normal_session_error) = kaltura_check_server_status($admin = 0);
  $status_class = ($admin_session_status && $normal_session_status)? 'ok': 'bad';
  $session_status = ($admin_session_status && $normal_session_status)? 'OK': 'ERROR';
  $error_status = FALSE;

  $error = ' kaltura_error: ';
  if ($admin_session_error) {
    $error .= '<br />admin session: '. $admin_session_error;
    $error_status = TRUE;
  }
  if ($normal_session_error) {
    $error .= '<br />normal session: '. $normal_session_error;
    $error_status = TRUE;
  }

  $form['kaltura_server_status']['kaltura_Admin_test_server'] = array(
    '#type' => 'item',
    '#title' => t('Drupal to Kaltura Session Test'),
    '#prefix' => '<div class="kaltura_status_'. $status_class .'">',
    '#markup' => 'Session status: '. $session_status . (($error_status)? $error: ''),
    '#suffix' => '</div>'
    );
    /*
     *if (kaltura_variable_get_real('kaltura_notification_type', 0) == 1) {
     *  list($status, $text) = kaltura_test_notification_config();
     *  $prefix = '<div class="kaltura_status_'. (($status)? 'ok': 'bad') .'">';
     *  $form['kaltura_server_status']['kaltura_notification_status'] = array(
     *  '#type' => 'item',
     *  '#title' => t('Kaltura to Drupal Notification Status'),
     *  '#prefix' => $prefix,
     *  '#markup' => $text,
     *  '#suffix' => '</div>'
     *  );
     *}
     */
    list($status, $text) = kaltura_test_cron_job_status();
    $prefix = '<div class="kaltura_status_'. (($status)? 'ok': 'bad') .'">';
    $form['kaltura_server_status']['kaltura_cron_job_status'] = array(
    '#type' => 'item',
    '#title' => t('Cron-Job Status'),
    '#prefix' => $prefix,
    '#markup' => $text,
    '#suffix' => '</div>'
    );
    list($status, $text) = kaltura_test_crossdomain();
    $prefix = '<div class="kaltura_status_'. (($status)? 'ok': 'bad') .'">';
    $form['kaltura_server_status']['kaltura_crossdomain_status'] = array(
    '#type' => 'item',
    '#title' => t('CrossDomain.xml Status'),
    '#prefix' => $prefix,
    '#markup' => $text,
    '#suffix' => '</div>'
  );
  $form['kaltura_partner_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Partner Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['kaltura_partner_settings']['kaltura_partner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner ID'),
    '#default_value' => variable_get('kaltura_partner_id', ''),
    '#size' => 20,
    '#maxlength' => 10,
    '#disabled' => TRUE
  );
  $form['kaltura_partner_settings']['cms_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => variable_get('kaltura_partner_cms_email', ''),
    '#disabled' => TRUE
  );
  $form['kaltura_partner_settings']['cms_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('kaltura_partner_cms_password', ''),
    '#disabled' => TRUE
  );

    return $form;

}

function kaltura_settings_form() {
  $html5_desc_text_ce = t('Contact your Kaltura server administrator to enable HTML5 players');
  $html5_desc_text_saas = t('Contact Kaltura Sales to enable HTML5 players ') . l('http://site.kaltura.com/contact-us-drupal.html', 'http://site.kaltura.com/contact-us-drupal.html');
  $htm5_desc = (variable_get('kaltura_server_url', '') == 'http://www.kaltura.com') ? $html5_desc_text_saas : $html5_desc_text_ce;
  $mobile = FALSE;
  $mobile = KalturaHelpers::hasMobileFlavores();
  $form['kaltura_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kaltura general settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('You can perform various advanced administration actions on your media content via Kaltura Management Console. These actions include: setting access control, changing thumbnails, create players, update your email and password, et cetera. Access Kaltura Management Console here:' . l('http://www.kaltura.com/index.php/kmc', 'http://www.kaltura.com/index.php/kmc')),
  );
  $form['kaltura_settings']['kaltura_html5'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML5 settings mobile delivery'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['kaltura_settings']['kaltura_html5']['add_html5'] = array(
    '#type' => 'checkbox',
    '#title' => t('Support iPhone and iPad with HTML5'),
    '#default_value' => variable_get('kaltura_add_html5', $mobile),
    '#description' => t('The HTML5 video standard is supported in many mobile devices such as Apple’s iPhone and version 2 of the Google Android platform. Some of these popular mobile platforms don’t support Flash plugins and subsequently the video tag is the exclusive mechanism to distribute video in these mobile HTML browsers.</br>
    By enabling this configuration, the Kaltura videos of this website would play on these devices.'),
    '#disabled' => !$mobile,
  );
  if ($mobile == FALSE) {
    $form['kaltura_settings']['kaltura_html5']['html5_text'] = array(
      '#markup' => $htm5_desc,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );
  return $form;

}

function kaltura_settings_form_submit($form, $form_state) {
  $vals = $form_state['values'];
  variable_set('kaltura_add_html5', $vals['add_html5']);

}

/*
 * callback function for url admin/settings/kaltura/import
 *CMAC
 *changes
 * KalturaEntryFilter changed to KalturaMediaEntryFilter
 * this may need some reworking
 */
function kaltura_import_entries_page() {
  // initialise variables
  $foundFlag = true;      // set to false if there are no entries
  $entries = array();     // mix entries
  $mediaEntries = array();  // media entries
  $entries_need_import = array();
  $each_page=30;
  $total = 0;
  $mediaTotal = 0;
  $finished = false;
  $current_page = 1;
  $pager = '';

  if (!empty($_POST['op']) && $_POST['op'] == 'import selected') {
    kaltura_import_entries($_POST['entries_to_import']);
    drupal_goto('admin/config/media/kaltura/import');
  }
  if (variable_get('kaltura_partner_id', '') == '') {
    return t('You have not configured your partner details yet. Import cannot be performed until you setup your partner details');
  }

  if (empty($_GET['p'])) {
    $_GET['p'] = 1;
  }
  else {
    $current_page = $_GET['p'];
  }

  while (!$finished) {
    set_time_limit(30);
    // get the media entries
      list($mediaTotal, $mediaEntries) = kaltura_get_my_entries($each_page, $current_page);
      if ($mediaTotal == 0) {
        $foundFlag=false;
      }
      else {
        $enabledFlag = true;
      }
    $entries_need_import = array_merge($entries_need_import, $entries, $mediaEntries);

    //list($total, $each_page, $entries) = kaltura_get_my_entries($each_page = 30, $_GET['p']);
    /*
     *if ($entries === NULL) {
     *  return t('No entries found.');
     *}
     */
    $entries_need_import = kaltura_remove_existing_entries($entries_need_import);

    if (count($entries_need_import) >= $each_page)
    {
      $finished = true;
    }

    if (($mediaTotal < $current_page * $each_page) && ($total < $current_page * $each_page))
    {
      $finished = true;
    }
    $current_page++;
  }

  if (!count($entries_need_import)) {
    return t('No entries needed to be imported.');
  }

  if (count($entries_need_import) > $each_page)
  {
    $multi_dim = array_chunk  ( $entries_need_import  , $each_page, true );
    $entries_need_import = $multi_dim[0];
  }
/*
  if (count($entries_need_import) < $each_page) {
    list($second_count, $second_page) = kaltura_get_my_entries($filter, $each_page, $_GET['p'] + 1);
    $new_entries_need_import = kaltura_remove_existing_entries($second_page);
    if (is_array($new_entries_need_import) && $second_page !== NULL) {
      foreach ($new_entries_need_import as $eid => $entry) {
        $entries_need_import[$eid] = $entry;
        if (count($entries_need_import) == $each_page) {
          break;
        }
      }
    }
  }
*/
  if (($total+$mediaTotal) > $each_page) {
    $pages = ceil(($total+$mediaTotal)/$each_page);
    for ($start = 1;$start <= $pages;$start++) {
      if ($start == $_GET['p']) {
        $pager .= '<a>'. $start .'</a>';
      }
      else {
        $pager .= l($start, 'admin/config/media/kaltura/import', array('query' => array('p' => $start)));
      }
      $pager .= '&nbsp;';
    }
  }
  if (is_array($entries_need_import) && count($entries_need_import)) {
    $ksetings = new KalturaSettings;
    foreach ($entries_need_import as $entry => $details) {
      $entries_options[$details['id']] = $details['name'] .': ' . '(' . $ksetings->media_types_map[$details['type']] . ') - ' . $details['id'];
    }
  }
  else {
    $entries_options = array();
  }
  $output = drupal_get_form('kaltura_create_entries_form', array($entries_options));
  $output = drupal_render($output);
  return $pager . $output . $pager;
}

/*
 * helper function that uses kaltura_get_entries() to retreive a list of specific entries
 * belongs to the partner, and call kaltura_import_entry() for each of the retreived list
 */
function kaltura_import_entries($entries) {
  $entries_list = implode(',', $entries);
  $my_entries = kaltura_get_entries($entries, count($entries), true);

  foreach ($my_entries[0]->objects as $entry) {
    kaltura_import_entry($entry);
  }
}

/*
 * helper function that gets an entry retreived from Kaltura
 * builds a node object according to entry, and invokes node_insert to save the node
 */
function kaltura_import_entry($entry_details) {
 // global $user;
  $entry = kaltura_kmc_obj_to_drupal_array($entry_details);
  $entry['kaltura_created_date'] = REQUEST_TIME; //we have no way when the entry was created on the KMC so the create date is when we received the notifcation
  db_insert('node_kaltura')
      ->fields($entry)
      ->execute();
}



/**
 * kaltura_kmc_obj_to_drupal_array
 * this function gets and object retrived be the kaltura API and terns it to an entry array to be incerted in to the drupal db
 * @param mixed $obj
 * @access public
 * @return void
 */
function kaltura_kmc_obj_to_drupal_array($obj) {
  $ent['kaltura_entryid'] = $obj->id;
  $ent['kaltura_tags'] = $obj->tags;
  $ent['kaltura_admin_tags'] = $obj->adminTags;
  $ent['kstatus'] = $obj->status;
  $ent['kaltura_media_type'] = $obj->mediaType;
  $ent['kaltura_duration']= $obj->duration;
  $ent['kaltura_thumbnail_url'] = $obj->thumbnailUrl;
  $ent['kaltura_partner_data'] = $obj->partnerData;
  $ent['kaltura_source'] = $obj->sourceType;
  $ent['kaltura_width'] = $obj->width;
  $ent['kaltura_height'] = $obj->height;
  $ent['kaltura_download_url'] = $obj->downloadUrl;
  $ent['kaltura_views'] = $obj->views;
  $ent['kaltura_plays'] = $obj->plays;
  $ent['kaltura_votes'] = $obj->votes;
  $ent['kaltura_rank'] = $obj->rank;
  $ent['kaltura_total_rank'] = $obj->totalRank;
  $ent['kaltura_title'] = $obj->name;
  $ent['kaltura_description'] = $obj->description;
  return $ent;
}

/*
 * helper function to provide form fields containing all entries for import
 * each entry as checkbox option
 */
function kaltura_create_entries_form($form) {
  $args = func_get_args();
  $entries_options = $args[2][0];
  $form['desc'] = array (
    '#markup' => t('The list below includes media entries of your Kaltura account that are not synced to the Drupal server. Please select media entries to import to the Drupal server and submit. </br> Imported media would be available to use within the Drupal site.') . '</br></br>',
  );
  $form['check_all'] = array (
    '#type' => 'checkbox',
    '#title' => t('check all'),
    '#attributes' => array('class' => array('checkall')),
  );
  $form['entries_to_import'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose the entries you want to import'),
    '#options' => $entries_options,
  );
  $form['import_now'] = array(
    '#type' => 'submit',
    '#title' => t('import selected'),
    '#value' => t('import selected'),
  );
  return $form;
}

/*
 * helper functuion to get all entries of partner from Kaltura's DB
 * TODO
 * CMAC
 * changes:
 * now retrieving all media entries - needs to import mix entries as well
 */
function kaltura_get_my_entries($page_size = 30, $page = 1) {
  $isAdmin = 1;
  $session_user = KalturaHelpers::getSessionUser();
  $kaltura_client = KalturaHelpers::getKalturaClient($isAdmin); // this is admin task and should be able to import all entries in the account
  $pager_filter = new KalturaFilterPager();
  $kmf = new KalturaBaseEntryFilter();
  $kmf->typeEqual = KalturaEntryType::MEDIA_CLIP;
  $pager_filter->pageIndex = $page;
  $pager_filter->pageSize = $page_size;
  $result = $kaltura_client->baseEntry->listAction($kmf, $pager_filter);
  if ($result->totalCount > 0) {
    $count = $result->totalCount;
    if (is_array($result->objects) && count($result->objects)) {
      foreach ($result->objects as $entry) {
        $returned_entries[$entry->id] = array(
                'id' => $entry->id,
                'name' => $entry->name,
                'type' => $entry->mediaType,
        );
      }
    }
    return array($count, $returned_entries);
  }
  else {
    return array(0, array());
  }
}

/*
 * helper function that recieves a list of entries, and returns a new list
 * which does not contain entries that are already represented as nodes
 */
function kaltura_remove_existing_entries($entries) {
  $count = 0;
  $new_entries = array();
  if (is_array($entries) && count($entries)) {
    foreach ($entries as $entry) {
      $sel = db_select('node_kaltura', 'k')
        ->fields('k', array('kaltura_entryid'))
        ->condition('kaltura_entryid', $entry['id'], '=')
        ->countQuery()
        ->execute()
        ->fetchField();
      //$sel = 'SELECT COUNT(nid) n FROM {node_kaltura} WHERE kaltura_entryId = \'%s\'';
      //$result = db_fetch_object(db_query($sel, $entry['id']));
      if ($sel == 0) {
        $new_entries[] = $entry;
      }
      else {
        $count++;
      }
    }
  }
  return $new_entries;
}


/*
 * helper function that checks the drupal cron job status
 * cron job is required to collect statictics about kaltura items
 */
function kaltura_test_cron_job_status() {
  $cron_last = variable_get('cron_last', NULL);
  if (!$cron_last) {
    return array(
    FALSE,
      'cron has not run. If you don\'t configure cron job, statistics about kaltura items will not be updated.'
      );
  }
  else {
    return array(
    TRUE,
      'cron has run. Be sure that you have a cron job configured, so you don\'t need to run it manually.<br />'.
      'Statistics about kaltura items will only be updated at cron run.'
      );
  }
}

/*
 * helper function that tests the notification status of the module
 *
 * this function tries to close a loop with the kaltura sevrer in a "real-time" test.
 * The notification handler sets a variable in the DB before the this function run ends
 * therefore we need to use the kaltura_variable_get_real() function to check the variable status in the DB
 * and not in the $_GLOBALS which are set before this function even start
 * TODO
 * CMAC
 * not yet implemented
 */
function kaltura_test_notification_config() {
  return array(TRUE, 'Test call not yet implemented, sorry');
  if (kaltura_variable_get_real('kaltura_notification_status', 0) == 0) {
    $session_user = KalturaHelpers::getSessionUser();
    $kaltura_client = KalturaHelpers::getKalturaClient();
    $result = $kaltura_client->testNotification($session_user);
    sleep(3);
  }
  if (kaltura_variable_get_real('kaltura_notification_status', 0) === 1) {
    return array(TRUE, 'Drupal server receives notification from Kaltura server.');
  }

  return array(FALSE, 'Drupal server fails to receive notifications from Kaltura. Please fix your configuration or disable server notifications.');
}

/*
 * replace default Drupal's variable_get() function with a "real-time" one
 *
 * This function checks a variable status in the DB and not in the $_GLOBALS
 * TODO: way do we need this one????
 */
function kaltura_variable_get_real($var, $default) {
  $result = db_select('variable', 'v')
    ->fields('v')
    ->condition('name', $var, '=')
    ->execute()
    ->fetchAssoc();
  if (!$result) {
    return $default;
  }

  return unserialize($result['value']);
}

/**
 * Used when SimpleXML is not available; just assume everything looks OK and hope for the best.
 */
function kaltura_alternate_xml_verification($filename) {
  return array(
    TRUE,
    t('crossdomain.xml is in place and seems to be OK')
  );
}

/**
 * helper function that checks if crossdomain.xml is in the right place
 */
function kaltura_test_crossdomain() {
  global $base_url ;
  $cd_req = drupal_http_request($base_url .'/crossdomain.xml');
  if ( $cd_req->code == '200' ) {
    return kaltura_parse_crossdomain($cd_req->data);
  }
  else {
    // fall back to fetching and retrieving file contents (should we
    // succeed if HTTP request has failed, though?
    $filename = $_SERVER['DOCUMENT_ROOT'] .'/crossdomain.xml';
    if ( $cd_xml = file_get_contents($filename) ) {
      return kaltura_parse_crossdomain($cd_xml) ;
    }
    else {
      return array(
        FALSE,
        t('crossdomain.xml file could not be found in your site\'s root directory') .' ['. $filename .']' .
        t('Please read ') . l('here', 'admin/help/kaltura')
      );
    }
  }
}

/**
 * helper function that checks if crossdomain.xml says the right things
 */
function kaltura_parse_crossdomain($crossdomain_xml) {
  if (!function_exists('simplexml_load_string')) {
    return kaltura_alternate_xml_verification($filename);
  }
  $cd_xml_content = simplexml_load_string($crossdomain_xml);
  foreach ($cd_xml_content->children() as $child) {
    $atts = $child->attributes();
    if ($atts['domain'] == '*' || $atts['domain'] == '*.kaltura.com') {
      $kaltura_ok = TRUE;
    }
    if ($atts['headers'] == '*' && ($atts['domain'] == '*' || $atts['domain'] == '*.kaltura.com')) {
      $headers_ok = TRUE;
    }
  }
  if ($kaltura_ok && $headers_ok) {
    return array(
      TRUE,
      t('crossdomain.xml is in place and seems to be OK')
    );
  }
  else {
    return array(
      FALSE,
      t('crossdomain.xml is in place, but it is probably not configured properly') .'<br />'.
      t('Please read ') . l('here', 'admin/help/kaltura')
    );
  }
}

/*
 * helper function that updates partner notification settings in kaltura's server
 *
 * CMAC
 * changes
 */
function kaltura_update_kaltura_partner($notifications_config = 2, $notification_url = '') {
  $partner = new KalturaPartner();
  $partner->allowMultiNotification = '1';
  $partner->notify = 1;
  $partner->notificationsConfig = '*='. $notifications_config;
//  $partner->id = variable_get('kaltura_partner_id', '');

  $url = url('kaltura/notification_handler', array('absolute' => TRUE));
  $notification_url = ($notification_url == '')? $url: $notification_url;
  $partner->notificationUrl = $notification_url;
  $session_user = KalturaHelpers::getSessionUser();
  $kaltura_client = KalturaHelpers::getKalturaClient(1);
  $cms_pass = variable_get('kaltura_partner_cms_password', '');

  if ($kaltura_client) {
    $result = $kaltura_client->partner->update($partner, $cms_pass);
    //$result = $kaltura_client->updatePartner($session_user, $partner);
  }
  return $result;
}

/*
 * helper function that tries to open session with the kaltura server
 *
 * used to verify partner details, and display errors if there are
 *
 * CMAC
 * changed
 * added new variable_get statements required for the api calls
 * updated KalturaClient creation process
 * changed API calls
 *
 * TODO check correct format for error checking of result
 */
function kaltura_check_server_status($admin) {
  try
  {
    $result = KalturaHelpers::getKalturaClient($admin);
    $kaltura_status = TRUE;
    $kaltura_error = "";
  }
  catch(Exception $ex)
  {
    $kaltura_status = FALSE;
    $kaltura_error = $ex->getMessage();
  }
  return array($kaltura_status, $kaltura_error);
}

/*
 * helper function to generate a cms password
 *
 * cms is the "administration panel" for kaltura's partners in kaltura's website
 */
function kaltura_generate_cms_password() {
  $pass = '';
  $pass = md5(time() . $_SERVER['REMOTE_ADDR']);
  $pass = drupal_substr($pass, 0, 8);
  return $pass;
}
