<?php

// Set up our query
//$query = '{"jsonrpc":"2.0","method":"condenser_api.get_discussions_by_blog","params":[{"tag":"makerhacks","limit":30}],"id":0}';

$query = '{"jsonrpc":"2.0","method":"condenser_api.get_discussions_by_created","params":[{"tag":"hive-103257","limit":30,}],"id":0}';


$ch = curl_init("https://api.hive.blog");
curl_setopt($ch, CURLOPT_POSTFIELDS, $query);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

// Get the response from the API
$response = curl_exec($ch);

// Close the connection
curl_close($ch);

// Get the blog posts
$posts=json_decode($response, TRUE);
//print_r($posts);

// Iterate through the returned posts
for($x = 0; $x < count($posts['result']); $x++) {

    // Pull out specifics
    $post=$posts['result'][$x];
    $meta=json_decode($post['json_metadata'],TRUE);
    $image=$meta['image'];
    $tags=join(', ',$meta['tags']);

    // Don't Show Cross-Posts
    if(stristr($tags, 'cross-post')==FALSE){
        $link = "https://peakd.com/@".$post['author']."/".$post['permlink'];

        print($post['post_id']);

        // Output HTML
        $content .= '
            <h2><a href="'.$link.'" target="_blank">'.$post['title'].'</a></h2>
            <p>'.$tags.'</p>
            <p><a href="'.$link.'" target="_blank"><img src="'.$image[0].'" width="300"></a></p>';
    }
    
}

print($content);

?>