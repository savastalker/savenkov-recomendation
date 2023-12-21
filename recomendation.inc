<?php
/*
Plugin Name: Savenkov-recomendation
Plugin URI: https://example.com
Description: A plugin that provides personalized product recommendations for online shoppers
Version: 1.0
Author: Your Name
Author URI: https://example.com
License: GPL2
*/

// Define some constants
define( 'RSP_PLUGIN_DIR', plugin_dir_path( __FILE__ ) );
define( 'RSP_PLUGIN_URL', plugin_dir_url( __FILE__ ) );
define( 'RSP_MAX_RECOMMENDATIONS', 5 ); // The maximum number of products to recommend

// Include some files
include_once RSP_PLUGIN_DIR . 'includes/class-rsp-recommender.php'; // The class that implements the recommendation algorithm
include_once RSP_PLUGIN_DIR . 'includes/class-rsp-widget.php'; // The class that creates the recommendation widget

// Register the widget
function rsp_register_widget() {
register_widget( 'RSP_Widget' );
}
add_action( 'widgets_init', 'rsp_register_widget' );

// Enqueue some scripts and styles
function rsp_enqueue_scripts() {
wp_enqueue_style( 'rsp-style', RSP_PLUGIN_URL . 'assets/css/rsp-style.css' ); // The style for the widget
wp_enqueue_script( 'rsp-script', RSP_PLUGIN_URL . 'assets/js/rsp-script.js', array( 'jquery' ), false, true ); // The script for the widget
}
add_action( 'wp_enqueue_scripts', 'rsp_enqueue_scripts' );

// Create a global instance of the recommender class
global $rsp_recommender;
$rsp_recommender = new RSP_Recommender();

// Hook into some actions and filters
add_action( 'woocommerce_single_product_summary', array( $rsp_recommender, 'rsp_display_recommendations' ), 25 ); // Display the recommendations on the single product page
add_action( 'woocommerce_thankyou', array( $rsp_recommender, 'rsp_update_user_profile' ), 10, 1 ); // Update the user profile after a purchase
add_filter( 'the_content', array( $rsp_recommender, 'rsp_add_tracking_code' ) ); // Add the tracking code to the content
?>