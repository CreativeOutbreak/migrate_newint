<?php

/**
 * @file
 * Basic abstract classes to be inherited from 
 */

abstract class NewIntMigration extends Migration {
    protected $psql_table;
    protected $psql_alias;
    protected $psql_fields = array('id');
    protected $psql_query;
    public function __construct($group) {
         
        parent::__construct(array($group));
        $this->team = array(new MigrateTeamMember('Pete Stewart', 'pete@newint.org', t('Web Development Manager')));
        $this->issuePattern = 'http://www.hostedredmine.com/issues/:id:';
    }

    public function getConnection($conn = 'legacy') {
        return Database::getConnection('default',$conn);
    }
}


abstract class NewIntPSQLMigration extends NewIntMigration {
    
    public function __construct($group) {

        parent::__construct($group); 
        
        if(isset($this->psql_table) && isset($this->psql_alias)) { 
            $this->psql_query = $this->getConnection()->select($this->psql_table, $this->psql_alias)
                ->fields($this->psql_alias, $this->psql_fields);
        } 
        $count_query = $this->getConnection()->select($this->psql_table, $this->psql_alias);
        $count_query->addExpression('COUNT(*)', 'cnt');



        $this->source = new MigrateSourceSQL($this->psql_query, array(), NULL, array('map_joinable' => FALSE));
    }
}
/*
abstract class NewIntPSQLMigration extends NewIntMigration {
    protected $psql_table;
    protected $psql_alias;
    protected $psql_fields = array('id');
    protected $psql_query;
    protected $psql_count_q;
    public function __construct($group) {

        parent::__construct($group); 
        
        if(isset($this->psql_table) && isset($this->psql_alias)) { 
            $this->psql_query = $this->getConnection()->select($this->psql_table, $this->psql_alias)
                ->fields($this->psql_alias, $this->psql_fields);
        } 
        $this->psql_count_q = $this->getConnection()->select($this->psql_table, $this->psql_alias);
        $this->psql_count_q->addExpression('COUNT(*)', 'cnt');



        
    }

    public function SetSource() {
        $this->source = new MigrateSourceSQL($this->psql_query, array(), $this->psql_count_q, array('map_joinable' => FALSE));
    }
}
*/
