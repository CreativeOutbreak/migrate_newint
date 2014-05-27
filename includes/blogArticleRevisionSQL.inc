<?php

/**
 * @file
 * A basic example of using the Migrate module to import tags from bricolage to taxonomy
 * 
 */


class NewIntBlogArticleRevisionSQLMigration extends NewIntPSQLMigration {
    public function __construct($group) {
        $this->description = 'Pull all blog revisions from newint bricolage system';
        $this->dependencies = array('NewIntTermSQL', 'NewIntUserSQL','NewIntBlogArticleSQL');
        $this->psql_table = 'view_blog_article_wt_revisions';
        $this->psql_alias = 'b';
        $this->psql_fields = array('id', 'version', 'usr__id', 'slug', 'note', 'name', 'teaser', 'body_text', 'first_publish_date', 'publish_date', 'publish_status', 'keywords', 'primary_uri', 'new_uri', 'publish_time');
        parent::__construct(MigrateGroup::getInstance('blog'));

        $this->psql_query = $this->getConnection()->select($this->psql_table, $this->psql_alias)
                ->fields($this->psql_alias, $this->psql_fields);

        $this->psql_query->leftJoin('migrate_map_newintusersql', 't8', "b.usr__id = t8.sourceid1");
        $this->psql_query->addField('t8', 'destid1', 'usr_destid');
        
        $this->psql_query->leftJoin('migrate_map_newintblogarticlesql', 't9', "b.id = t9.sourceid1");
        $this->psql_query->addField('t9', 'destid1', 'nid');
        
        $this->source = new MigrateSourceSQL($this->psql_query, array(), NULL, array('map_joinable' => FALSE));



        $this->destination = new MigrateDestinationNodeRevision('blog');

        $this->map = new MigrateSQLMap($this->machineName,
            array(
                'id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'b',
                )
            ),
            MigrateDestinationNodeRevision::getKeySchema(),
            'legacy'
        );




        $this->addFieldMapping('title', 'name');
        //$this->addFieldMapping('uid', 'usr_destid');
        //$this->addFieldMapping('created', 'first_publish_date');
        $this->addFieldMapping('changed', 'publish_time');
        $this->addFieldMapping('status', 'publish_status')->defaultValue(1);
        $this->addFieldMapping('revision', NULL)->defaultValue(1);
        $this->addFieldMapping('log', 'note')->defaultValue(t('New Revision'));
        $this->addFieldMapping('revision_uid', 'usr_destid');
        $this->addFieldMapping('body', 'body_text');
        $this->addFieldMapping('body:summary', 'teaser');
        $this->addFieldMapping('body:format', NULL)->defaultValue('filtered_html');
        $this->addFieldMapping('field_blog_image', 'new_uri');
        $this->addFieldMapping('field_blog_tags', 'keywords'); 
        $this->addFieldMapping('field_blog_tags:ignore_case', NULL)->defaultValue('1');
        $this->addFieldMapping('field_blog_tags:create_term', NULL)->defaultValue('1');
        $this->addFieldMapping('nid', 'nid');
        $this->addFieldMapping('vid', 'version');
        //$this->addFieldMapping('field_blog_tags', 'keywords');
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
            'created',
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

    public function is_image($path) {
        $a = getimagesize($path);
        $image_type = $a[2];
         
        if(in_array($image_type , array(IMAGETYPE_GIF , IMAGETYPE_JPEG ,IMAGETYPE_PNG , IMAGETYPE_BMP)))
        {
            return true;
        }
        return false;
    }

    public function prepareRow($row) {
        
        /*if(!isset($row->publish_time)) {
            $day = "+";
            $day .= $row->version;
            $day .= " days";
            $time = strtotime($day, strtotime($row->first_publish_date));
            $day = date('Y-m-d H:i:s', $time);
            $row->publish_time = $day;
            //dd($row->publish_time); 
        }*/
        //dd("to: ".$row->publish_time." from: ".$row->first_publish_date);
        dd($row);
        $row->publish_status = ($row->publish_status === 'f' ? '0' : '1');
        $row->keywords = explode(',', $row->keywords);
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

