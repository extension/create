set :deploy_to, '/services/apache/vhosts/dev.create.extension.org/docroot/'
set :branch, 'develop'
server 'dev.create.extension.org', :app, :web, :db, :primary => true