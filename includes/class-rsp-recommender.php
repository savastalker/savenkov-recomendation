<?php
// The class that implements the recommendation algorithm
class RSP_Recommender {

// The constructor
public function __construct() {
// Initialize some properties
$this->user_id = get_current_user_id(); // The current user ID
$this->product_id = get_the_ID(); // The current product ID
$this->user_profile = get_user_meta( $this->user_id, 'rsp_user_profile', true ); // The user profile data
$this->product_profile = get_post_meta( $this->product_id, 'rsp_product_profile', true ); // The product profile data
$this->recommendations = array(); // The array of recommended products
}

// The method that displays the recommendations on the single product page
public function rsp_display_recommendations() {
// Check if the current page is a single product page
if ( is_singular( 'product' ) ) {
// Generate the recommendations
$this->rsp_generate_recommendations();
// Check if there are any recommendations
if ( ! empty( $this->recommendations ) ) {
// Display the recommendations
echo '<div class="rsp-recommendations">';
echo '<h3>Вам також може сподобатися</h3>';
echo '<ul>';
foreach ( $this->recommendations as $recommendation ) {
// Get the product data
$product = wc_get_product( $recommendation['id'] );
$title = $product->get_title();
$price = $product->get_price_html();
$link = $product->get_permalink();
$image = $product->get_image();
// Display the product
echo '<li>';
echo '<a href="' . $link . '">';
echo $image;
echo '<h4>' . $title . '</h4>';
echo '<span class="price">' . $price . '</span>';
echo '</a>';
echo '</li>';
}
echo '</ul>';
echo '</div>';
}
}
}

// The method that generates the recommendations
public function rsp_generate_recommendations() {
// Check if the user profile and the product profile are available
if ( $this->user_profile && $this->product_profile ) {
// Get the products that are similar to the current product
$similar_products = $this->rsp_get_similar_products();
// Get the products that the user may like based on their history
$user_liked_products = $this->rsp_get_user_liked_products();
// Merge the two arrays and remove duplicates
$merged_products = array_unique( array_merge( $similar_products, $user_liked_products ), SORT_REGULAR );
// Sort the products by their score in descending order
usort( $merged_products, function( $a, $b ) {
return $b['score'] - $a['score'];
});
// Get the top N products, where N is the maximum number of recommendations
$top_products = array_slice( $merged_products, 0, RSP_MAX_RECOMMENDATIONS );
// Set the recommendations property
$this->recommendations = $top_products;
}
}

// The method that gets the products that are similar to the current product
public function rsp_get_similar_products() {
// Initialize an empty array
$similar_products = array();
// Get all the products in the same category as the current product
$category_products = $this->rsp_get_category_products();
// Loop through each product
foreach ( $category_products as $category_product ) {
// Get the product ID
$product_id = $category_product->ID;
// Skip the current product
if ( $product_id == $this->product_id ) {
continue;
}
// Get the product profile data
$product_profile = get_post_meta( $product_id, 'rsp_product_profile', true );
// Check if the product profile is available
if ( $product_profile ) {
// Calculate the similarity score between the current product and the category product
$similarity_score = $this->rsp_calculate_similarity( $this->product_profile, $product_profile );
// Add the product and its score to the array
$similar_products[] = array(
'id' => $product_id,
'score' => $similarity_score
);
}
}
// Return the array of similar products
return $similar_products;
}

// The method that gets the products that the user may like based on their history
public function rsp_get_user_liked_products() {
// Initialize an empty array
$user_liked_products = array();
// Get all the products that the user has viewed or purchased
$history_products = $this->rsp_get_history_products();
// Loop through each product
foreach ( $history_products as $history_product ) {
// Get the product ID
$product_id = $history_product->ID;
// Skip the current product
if ( $product_id == $this->product_id ) {
continue;
}
// Get the product profile data
$product_profile = get_post_meta( $product_id, 'rsp_product_profile', true );
// Check if the product profile is available
if ( $product_profile ) {
// Calculate the preference score between the user and the product
$preference_score = $this->rsp_calculate_preference( $this->user_profile, $product_profile );
// Add the product and its score to the array
$user_liked_products[] = array(
'id' => $product_id,
'score' => $preference_score
);
}
}
// Return the array of user liked products
return $user_liked_products;
}

// The method that gets all the products in the same category as the current product
public function rsp_get_category_products() {
// Initialize an empty array
$category_products = array();
// Get the current product categories
$product_categories = wp_get_post_terms( $this->product_id, 'product_cat' );
// Check if the product has any categories
if ( ! empty( $product_categories ) && ! is_wp_error( $product_categories ) ) {
// Get the first category ID
$category_id = $product_categories[0]->term_id;
// Get the products in the same category
$category_products = get_posts( array(
'post_type' => 'product',
'posts_per_page' => -1,
'tax_query' => array(
array(
'taxonomy' => 'product_cat',
'field' => 'term_id',
'terms' => $category_id
)
)
));
}
// Return the array of category products
return $category_products;
}

// The method that gets all the products that the user has viewed or purchased
public function rsp_get_history_products() {
// Initialize an empty array
$history_products = array();
// Get the products that the user has viewed
$viewed_products = $this->rsp_get_viewed_products();
// Get the products that the user has purchased
$purchased_products = $this->rsp_get_purchased_products();
// Merge the two arrays and remove duplicates
$history_products = array_unique( array_merge( $viewed_products, $purchased_products ), SORT_REGULAR );
// Return the array of history products
return $history_products;
}

// The method that gets the products that the user has viewed
public function rsp_get_viewed_products() {
// Initialize an empty array
$viewed_products = array();
// Get the user viewed products data
$user_viewed_products = get_user_meta( $this->user_id, 'rsp_user_viewed_products', true );
// Check if the user viewed products data is available
if ( $user_viewed_products ) {
// Loop through each product ID
foreach ( $user_viewed_products as $product_id ) {
// Get the product object
$product = get_post( $product_id );
// Check if the product object is valid
if ( $product && ! is_wp_error( $product ) ) {
// Add the product to the array
$viewed_products[] = $product;
}
}
}
// Return the array of viewed products
return $viewed_products;
}

// The method that gets the products that the user has purchased
public function rsp_get_purchased_products() {
// Initialize an empty array
$purchased_products = array();
// Get the user orders
$user_orders = wc_get_orders( array(
'customer' => $this->'customer' => $this->user_id,
'limit' => -1
));
// Loop through each order
foreach ( $user_orders as $user_order ) {
// Get the order items
$order_items = $user_order->get_items();
// Loop through each order item
foreach ( $order_items as $order_item ) {
// Get the product ID
$product_id = $order_item->get_product_id();
// Get the product object
$product = get_post( $product_id );
// Check if the product object is valid
if ( $product && ! is_wp_error( $product ) ) {
// Add the product to the array
$purchased_products[] = $product;
}
}
}
// Return the array of purchased products
return $purchased_products;
}

// The method that calculates the similarity score between two products
public function rsp_calculate_similarity( $product_profile_1, $product_profile_2 ) {
// Initialize the similarity score
$similarity_score = 0;
// Loop through each feature in the product profile
foreach ( $product_profile_1 as $feature => $value_1 ) {
// Check if the feature exists in the other product profile
if ( isset( $product_profile_2[$feature] ) ) {
// Get the value of the feature in the other product profile
$value_2 = $product_profile_2[$feature];
// Calculate the difference between the two values
$difference = abs( $value_1 - $value_2 );
// Calculate the weight of the feature based on its importance
$weight = $this->rsp_get_feature_weight( $feature );
// Calculate the contribution of the feature to the similarity score
$contribution = $weight * ( 1 - $difference );
// Add the contribution to the similarity score
$similarity_score += $contribution;
}
}
// Return the similarity score
return $similarity_score;
}

// The method that calculates the preference score between a user and a product
public function rsp_calculate_preference( $user_profile, $product_profile ) {
// Initialize the preference score
$preference_score = 0;
// Loop through each feature in the user profile
foreach ( $user_profile as $feature => $value_1 ) {
// Check if the feature exists in the product profile
if ( isset( $product_profile[$feature] ) ) {
// Get the value of the feature in the product profile
$value_2 = $product_profile[$feature];
// Calculate the similarity between the two values
$similarity = 1 - abs( $value_1 - $value_2 );
// Calculate the weight of the feature based on its importance
$weight = $this->rsp_get_feature_weight( $feature );
// Calculate the contribution of the feature to the preference score
$contribution = $weight * $similarity;
// Add the contribution to the preference score
$preference_score += $contribution;
}
}
// Return the preference score
return $preference_score;
}

// The method that gets the weight of a feature based on its importance
public function rsp_get_feature_weight( $feature ) {
// Initialize the weight
$weight = 0;
// Define an array of feature weights
$feature_weights = array(
'price' => 0.2, // The price of the product
'rating' => 0.2, // The average rating of the product
'reviews' => 0.1, // The number of reviews of the product
'color' => 0.1, // The color of the product
'size' => 0.1, // The size of the product
'material' => 0.1, // The material of the product
'style' => 0.1, // The style of the product
'brand' => 0.1 // The brand of the product
);
// Check if the feature exists in the array
if ( isset( $feature_weights[$feature] ) ) {
// Get the weight of the feature
$weight = $feature_weights[$feature];
}
// Return the weight
return $weight;
}

// The method that updates the user profile after a purchase
public function rsp_update_user_profile( $order_id ) {
// Get the order object
$order = wc_get_order( $order_id );
// Check if the order object is valid
if ( $order && ! is_wp_error( $order ) ) {
// Get the user ID
$user_id = $order->get_customer_id();
// Get the user profile data
$user_profile = get_user_meta( $user_id, 'rsp_user_profile', true );
// Check if the user profile data is available
if ( ! $user_profile ) {
// Initialize an empty array
$user_profile = array();
}
// Get the order items
$order_items = $order->get_items();
// Loop through each order item
foreach ( $order_items as $order_item ) {
// Get the product ID
$product_id = $order_item->get_product_id();
// Get the product profile data
$product_profile = get_post_meta( $product_id, 'rsp_product_profile', true );
// Check if the product profile data is available
if ( $product_profile ) {
// Loop through each feature in the product profile
foreach ( $product_profile as $feature => $value ) {
// Check if the feature exists in the user profile
if ( isset( $user_profile[$feature] ) ) {
// Update the value of the feature in the user profile
$user_profile[$feature] = ( $user_profile[$feature] + $value ) / 2;
} else {
// Add the feature and its value to the user profile
$user_profile[$feature] = $value;
}
}
}
}
// Update the user profile data
update_user_meta( $user_id, 'rsp_user_profile', $user_profile );
}
}

// The method that adds the tracking code to the content
public function rsp_add_tracking_code( $content ) {
// Check if the current page is a single product page
if ( is_singular( 'product' ) ) {
// Get the user ID
$user_id = get_current_user_id();
// Get the product ID
$product_id = get_the_ID();
// Get the user viewed products data
$user_viewed_products = get_user_meta( $user_id, 'rsp_user_viewed_products', true );
// Check if the user viewed products data is available
if ( ! $user_viewed_products ) {
// Initialize an empty array
$user_viewed_products = array();
}
// Add the product ID to the array
$user_viewed_products[] = $product_id;
// Update the user viewed products data
update_user_meta( $user_id, 'rsp_user_viewed_products', $user_viewed_products );
// Add the tracking code to the content
$content .= '<script>console.log("Product ' . $product_id . ' viewed by user ' . $user_id . '");</script>';
}
// Return the content
return $content;
}
}
?>