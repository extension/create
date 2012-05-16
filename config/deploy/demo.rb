set :deploy_to, '/services/apache/vhosts/create.demo.extension.org/docroot/'
set :branch, 'develop'
server 'create.demo.extension.org', :app, :web, :db, :primary => true