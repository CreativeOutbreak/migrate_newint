<?php

/**
 * @file
 * A basic example of using the Migrate module to import tags from bricolage to taxonomy
 * 
 */


class NewIntBlogArticleSQLMigration extends NewIntPSQLMigration {
    public function __construct($group) {
        $this->description = 'Pull tags from legacy database that has been created from newint bricolage system';
        $this->psql_table = 'view_blog_article_v1';
        $this->psql_alias = 'b';
        $this->psql_fields = array('story__id', 'version', 'usr__id', 'slug', 'name', 'teaser', 'body_text', 'first_publish_date', 'publish_date', 'publish_status', 'keywords', 'primary_uri', 'new_uri');
        parent::__construct(MigrateGroup::getInstance('blog'));
        
        $this->destination = new MigrateDestinationNode('blog');

        $this->map = new MigrateSQLMap($this->machineName,
            array(
                'story__id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'b',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );




        $this->addFieldMapping('title', 'name');
        //$this->addFieldMapping('uid', 'usr__id');

        $this->addFieldMapping('created', 'first_publish_date');
        $this->addFieldMapping('status', NULL)->defaultValue(1);
        $this->addFieldMapping('revision', 'version');
        $this->addFieldMapping('log', NULL)->defaultValue(t('First Import'));
        //$this->addFieldMapping('revision_uid', 'usr__id');
        $this->addFieldMapping('body', 'body_text');
        $this->addFieldMapping('body:summary', 'teaser');
        $this->addFieldMapping('body:format', NULL)->defaultValue('filtered_html');
        $this->addFieldMapping('field_blog_image', 'new_uri');
        $this->addFieldMapping('field_blog_tags', 'keywords'); //->arguments(array('source_type' => 'tid'));;
        //$this->addFieldMapping('field_blog_tags:source_type', NULL)->defaultValue('tid');
        $this->addFieldMapping('field_blog_tags:ignore_case', NULL)->defaultValue('1');
        $this->addFieldMapping('field_blog_tags:create_term', NULL)->defaultValue('1');
        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
        if (module_exists('path')) {
            $this->addFieldMapping('path', 'primary_uri');

            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto', 'primary_uri');
            }
        }
        
        $um_array = array(
            'uid',
            'revision_uid',
            'changed',
            'promote',
            'sticky',
            'language',
            'tnid',
            'translate',
            'is_new',
            'body:language',
            'field_blog_image:file_class',
            'field_blog_image:language',
            'field_blog_image:destination_dir',
            'field_blog_image:destination_file',
            'field_blog_image:file_replace',
            'field_blog_image:preserve_files',
            'field_blog_image:source_dir',
            'field_blog_image:alt',
            'field_blog_image:title',
            'field_blog_tags:source_type',
            'comment',
            'totalcount',
            'daycount',
            'timestamp'
        );
        foreach($um_array as $value) {
            $this->addFieldMapping($value)
                ->issueGroup(t('DNM'));
        }
    
    }

    public function is_image($path)
    {
        $a = getimagesize($path);
        $image_type = $a[2];
         
        if(in_array($image_type , array(IMAGETYPE_GIF , IMAGETYPE_JPEG ,IMAGETYPE_PNG , IMAGETYPE_BMP)))
        {
            return true;
        }
        return false;
    }
    public function prepareRow($row) {
        $row->keywords = explode(',', $row->keywords); //array('football');
        $row->primary_uri = rtrim(ltrim($row->primary_uri, '/'), '/');
        if(isset($row->new_uri)) {
            $file_headers = @get_headers($row->new_uri);
            if($file_headers[0] == 'HTTP/1.1 200 OK') {
                if(!$this->is_image($row->new_uri)) {
                    $row->new_uri = NULL;
                }
            }
            else {
                $row->new_uri = NULL;
            }
        }
    } 
}

