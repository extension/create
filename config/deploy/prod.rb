set :deploy_to, '/services/apache/vhosts/create.extension.org/docroot/'
set :branch, 'master'
server 'create.extension.org', :app, :web, :db, :primary => true