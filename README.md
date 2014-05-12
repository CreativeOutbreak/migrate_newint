New Internationalist Data Migrate Module
=======================================

This module is an extention of the drupal migrate module, created to migrate all the legacy data from Bricolage to Drupal.

Legacy Databases
---------------
We are using a dump of the live database take around the 30/04/14.
The database dump can be downloaded from [dev2.newint.org](http://dev2.newint.org/files/newint2.sql.tar.gz) UserName: file_admin  Password: RadicalFish7
We have also used [SchemaSpy](http://schemaspy.sourceforge.net/) to create a visual representation of the database relationship that can also be downloaded from [dev2.newint.org](http://dev2.newint.org/files/newint_org-db-structure.tar.gz)


PostgreSQL to MySQL/MariaDB
--------------------------
Our legacy data is in a PostgreSQL database and we are planning on have the new Drupal system running of MySQL/MariaDB, as MySQL is more widely used and should be easier to find skilled workers in the future.
This has made the migration setup slightly tricker, as you need to tell the migrate module not to link the databases, as it's not possible in this situation.

The way we've haddles this is to have an abstract class that deals with the database connection and makes sure that the module doesn't link cross database.
You can see this code in the second class in abstracts.php
