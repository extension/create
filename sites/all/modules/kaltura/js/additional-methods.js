jQuery.validator.addMethod("numberswithbasicpunc", function(value, element) {
    return this.optional(element) || /^[0-9-.,()'\"\s]+$/i.test(value);
}, "Numbers or punctuation only please");

jQuery(document).ready( function () {
          jQuery('#kaltura-registration-form').validate({
          wrapper: "span",
          onsubmit: false,
          rules: {
            first_name: {
              required: true
              },
            last_name: {
              required: true
              },
            email: {
              required: true,
              email: true
              },
            website: {
              url: true
              },
            phone: {
              required: true,
              numberswithbasicpunc: true
              },
            company: {
              required: true
              },
            title: {
              required: true
              },
            vertical_lead: {
              required: true
              },
            country: {
              required: true
              },
            terms: {
              required: true
              }
            },
          messages: {
            email: { email: "Plase enter a valid email"},
            phone: { numberswithbasicpunc: "Only Numbers and punctuations"}
            }
            //debug: true
          }
          );
          jQuery('.required').prev('label').css('color', 'red');
          }
       );
