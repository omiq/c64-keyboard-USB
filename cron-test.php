<?php
/**
 *
 * @link              https://hivewp.com
 * @since             1.0.0
 * @package           Hivewp_cron_test
 *
 * @wordpress-plugin
 * Plugin Name:       HiveWP_cron_test
 * Plugin URI:        https://hivewp.com
 * Description:       Imports the author's blog posts from the Hive Blockchain
 * Version:           1.0.0
 * Author:            Chris Garrett
 * Author URI:        https://hivewp.com
 * License:           GPL-2.0+
 * License URI:       http://www.gnu.org/licenses/gpl-2.0.txt
 */
function bl_print_tasks() {
    echo '<pre>'; 
    print_r( _get_cron_array() ); 
    echo '</pre>';
}

bl_print_tasks();