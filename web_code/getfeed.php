<?php //error_reporting(-1); ?>

<?php
if (isset($_GET['since_id'])) {
	require_once("twitteroauth/twitteroauth/twitteroauth.php"); //Path to twitteroauth library

	$consumerkey = "######################";
	$consumersecret = "############################################";
	$accesstoken = "#######################################################";
	$accesstokensecret = "############################################";
	 
	function getConnectionWithAccessToken($cons_key, $cons_secret, $oauth_token, $oauth_token_secret) {
	  $connection = new TwitterOAuth($cons_key, $cons_secret, $oauth_token, $oauth_token_secret);
	  return $connection;
	}
	 
	$connection = getConnectionWithAccessToken($consumerkey, $consumersecret, $accesstoken, $accesstokensecret);

	//$screen_name = 'pontifex';
	$screen_name = 'pontiflux_';
	//$since_id = '1';
	$since_id = trim($_GET['since_id']);
	//https://dev.twitter.com/docs/api/1.1/get/statuses/user_timeline
	$output = $connection->get("https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=".$screen_name."&since_id=".$since_id."&trim_user=true");
	//$output='398453667667263487';
	//var_dump($output);
	//print_r($output);
	/*
	States:
	1 = No since_id
	2 = Startup, send most recent id
	3 = No new tweet, do nothing
	4 = New tweet, lights on
	*/
	
	if(empty($output)){
	//if($output===$since_id){
		echo "<state>3</state>";
		echo "<sinceid>3</sinceid>";
	} else {
		if($since_id!='1'){
			echo "<state>4</state>";
		} else {
			echo "<state>2</state>";
		}
		//echo "<sinceid>398453667667263487</sinceid>";
		//echo "<sinceid>$output</sinceid>";
		echo "<sinceid>".$output[0]->id_str."</sinceid>";
	}

} else {
	echo "<state>1</state>";
	echo "<sinceid>1</sinceid>";	
}

?>