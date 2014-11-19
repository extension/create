set :stages, %w(prod dev)
set :default_stage, "dev"
require 'capistrano/ext/multistage'

require 'capatross'

set :application, "create"
set :repository,  "git@github.com:extension/drupal.git"
set :scm, "git"
set :user, "pacecar"
set :use_sudo, false
set :keep_releases, 3
ssh_options[:forward_agent] = true
set :port, 24
#ssh_options[:verbose] = :debug

before "deploy", "deploy:web:disable"
after "deploy:update_code", "deploy:link_and_copy_configs"
after "deploy:update_code", "deploy:cleanup"
after 'deploy',             'deploy:cacheclear'
after 'deploy',             'deploy:restart'
after 'deploy',             'deploy:web:enable'


namespace :deploy do

   # Override default restart task
   desc "Restart Apache"
   task :restart, :roles => :app do
     invoke_command '/usr/sbin/service apache2 restart', via: 'sudo'
   end

   # clear cache
   desc "Drush Cache Clear"
   task :cacheclear, :roles => :app do
     invoke_command "cd #{release_path} && /usr/bin/drush cache-clear all"
   end

  # Link up various configs (valid after an update code invocation)
  task :link_and_copy_configs, :roles => :app do
    run <<-CMD
    rm -rf #{release_path}/sites/default/settings.php &&
    rm -rf #{release_path}/sites/default/files &&
    rm #{release_path}/robots.txt &&
    ln -nfs #{shared_path}/config/robots.txt #{release_path}/robots.txt &&
    ln -nfs #{shared_path}/config/settings.php #{release_path}/sites/default/settings.php &&
    ln -nfs #{shared_path}/drupalfiles #{release_path}/sites/default/files
    CMD
  end

  [:start, :stop].each do |t|
    desc "#{t} task is a no-op with this application"
    task t, :roles => :app do ; end
  end


   # Override default web enable/disable tasks
   namespace :web do

      desc "Put Apache in maintenancemode by touching the maintenancemode file"
      task :disable, :roles => :app do
        invoke_command "touch /services/maintenance/#{vhost}.maintenancemode"
      end

      desc "Remove Apache from maintenancemode by removing the maintenancemode file"
      task :enable, :roles => :app do
        invoke_command "rm -f /services/maintenance/#{vhost}.maintenancemode"
      end

   end

end
