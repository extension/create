set :deploy_to, '/services/apache/vhosts/create.bootcamp.extension.org/docroot/'
set :branch, 'master'
server 'create.bootcamp.extension.org', :app, :web, :db, :primary => true