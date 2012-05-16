set :deploy_to, '/services/apache/vhosts/create.extension.org/docroot/'
set :branch, 'origin/master'
server 'create.extension.org', :app, :web, :db, :primary => true