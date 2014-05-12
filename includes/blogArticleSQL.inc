<?php

/**
 * @file
 * A basic example of using the Migrate module to import tags from bricolage to taxonomy
 * 
 */


class NewIntBlogArticleSQLMigration extends NewIntPSQLMigration {
    public function __construct($group) {
        $this->description = 'Pull tags from legacy database that has been created from newint bricolage system';
        $this->psql_table = 'view_blog_aticle_v1';
        $this->psql_alias = 'b';
        $this->psql_fields = array('story__id', 'version', 'usr__id', 'slug', 'name', 'teaser', 'body');
        parent::__construct(MigrateGroup::getInstance('blog'));
        
        $this->destination = new MigrateDestinationNode('blog');

        $this->map = new MigrateSQLMap($this->machineName,
            array(
                'nid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'b',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );




        /*$this->addFieldMapping('name', 'name');
        $this->addFieldMapping('description', 'description');

        $this->addFieldMapping('field_ni_tag_id', 'ni_id');


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
        if (module_exists('path')) {
            $this->addFieldMapping('path', 'path');

            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto', 'path');
            }
        }

        $this->addFieldMapping('weight')
            ->issueGroup(t('DNM'));
        $this->addFieldMapping('format')
            ->issueGroup(t('DNM'));
        $this->addFieldMapping('parent')
            ->issueGroup(t('DNM'));
        $this->addFieldMapping('parent_name')
            ->issueGroup(t('DNM'));
    */
    }
}

