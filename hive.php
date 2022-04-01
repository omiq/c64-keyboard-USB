<?php
/**
 *
 * @link              https://hivewp.com
 * @since             1.0.0
 * @package           Hivewp_list
 *
 * @wordpress-plugin
 * Plugin Name:       HiveWP_list
 * Plugin URI:        https://hivewp.com
 * Description:       Lists the author's blog posts from the Hive Blockchain
 * Version:           1.0.0
 * Author:            Chris Garrett
 * Author URI:        https://hivewp.com
 * License:           GPL-2.0+
 * License URI:       http://www.gnu.org/licenses/gpl-2.0.txt
 */


    function hivewp_list($atts) {

        // Set up our query
        $query = '{"jsonrpc":"2.0","method":"condenser_api.get_discussions_by_blog","params":[{"tag":"makerhacks","limit":30}],"id":0}';
        $ch = curl_init("https://api.hive.blog");
        curl_setopt($ch, CURLOPT_POSTFIELDS, $query);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

        // Get the response from the API
        $response = curl_exec($ch);

        // Close the connection
        curl_close($ch);

        // Get the blog posts
        $posts=json_decode($response, TRUE);

        // Iterate through the returned posts
        for($x = 0; $x < count($posts['result']); $x++) {

            // Pull out specifics
            $post=$posts['result'][$x];
            $meta=json_decode($post['json_metadata'],TRUE);
            $image=$meta['image'];
            $tags=join(', ',$meta['tags']);

            // Don't Show Cross-Posts
            if(stristr($tags, 'cross-post')==FALSE){
                $link = "https://peakd.com/@makerhacks/".$post['permlink'];

                // Output HTML
                $content = '
                    <h2><a href="'.$link.'" target="_blank">'.$post['title'].'</a></h2>
                    <p>'.$tags.'</p>
                    <p><a href="'.$link.'" target="_blank"><img src="'.$image[0].'" width="300"></a></p>';
            }
        }

        return $content;
    }
    
    add_shortcode('hivewp_list', 'hivewp_list');
    
?>