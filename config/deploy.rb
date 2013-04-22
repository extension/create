set :stages, %w(prod dev)
set :default_stage, "dev"
require 'capistrano/ext/multistage'

require 'capatross'
 
set :application, "drupal"
set :repository,  "git@github.com:extension/drupal.git"
set :scm, "git"
set :user, "pacecar"
set :use_sudo, false
set :keep_releases, 3
ssh_options[:forward_agent] = true
set :port, 24
#ssh_options[:verbose] = :debug

after "deploy:update_code", "deploy:link_and_copy_configs"
after "deploy:update_code", "deploy:cleanup"


namespace :deploy do
  
  
  # Link up various configs (valid after an update code invocation)
  task :link_and_copy_configs, :roles => :app do
    run <<-CMD
    rm -rf #{release_path}/sites/default/settings.php &&
    rm -rf #{release_path}/sites/default/files &&
    rm #{release_path}/robots.txt &&
    ln -nfs /services/config/#{application}/robots.txt #{release_path}/robots.txt &&
    ln -nfs /services/config/#{application}/settings.php #{release_path}/sites/default/settings.php &&
    ln -nfs /services/nfs/drupalfiles/files #{release_path}/sites/default/files
    CMD
  end

end

