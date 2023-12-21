<?php
// The class that creates the recommendation widget
class RSP_Widget extends WP_Widget {

// The constructor
public function __construct() {
// Initialize the widget with some options
parent::__construct(
'rsp_widget', // The widget ID
'Рекомендаційна система', // The widget name
array( 'description' => 'Відображає персоналізовані рекомендації товарів для покупців' ) // The widget options
);
}

// The method that outputs the widget content on the front-end
public function widget( $args, $instance ) {
// Extract the arguments
extract( $args );
// Get the widget title
$title = apply_filters( 'widget_title', $instance['title'] );
// Display the widget before content
echo $before_widget;
// Display the widget title if it exists
if ( ! empty( $title ) ) {
echo $before_title . $title . $after_title;
}
// Display the widget content
echo '<div class="rsp-widget-content">';
// Get the global instance of the recommender class
global $rsp_recommender;
// Display the recommendations
$rsp_recommender->rsp_display_recommendations();
echo '</div>';
// Display the widget after content
echo $after_widget;
}

// The method that outputs the widget settings form on the back-end
public function form( $instance ) {
// Get the widget title
$title = isset( $instance['title'] ) ? $instance['title'] : 'Рекомендовані товари';
// Display the title field
?>
<p>
<label for="<?php echo $this->get_field_id( 'title' ); ?>">Заголовок:</label>
<input class="widefat" id="<?php echo $this->get_field_id( 'title' ); ?>" name="<?php echo $this->get_field_name( 'title' ); ?>" type="text" value="<?php echo esc_attr( $title ); ?>">
</p>
<?php
}

// The method that updates the widget settings
public function update( $new_instance, $old_instance ) {
// Initialize an empty array
$instance = array();
// Sanitize and save the widget title
$instance['title'] = ( ! empty( $new_instance['title'] ) ) ? strip_tags( $new_instance['title'] ) : '';
// Return the instance
return $instance;
}
}
?>