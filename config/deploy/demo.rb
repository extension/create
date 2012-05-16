set :deploy_to, '/services/apache/vhosts/create.demo.extension.org/docroot/'
set :branch, 'origin/develop'
server 'create.demo.extension.org', :app, :web, :db, :primary => true