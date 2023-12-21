/* The script for the widget */
jQuery(document).ready(function($) {
// Get the widget element
var widget = $('.rsp-recommendations');
// Check if the widget exists
if (widget.length > 0) {
// Get the widget content element
var widgetContent = widget.find('.rsp-widget-content');
// Get the widget title element
var widgetTitle = widget.find('h3');
// Hide the widget content initially
widgetContent.hide();
// Add a click event listener to the widget title
widgetTitle.on('click', function() {
// Toggle the widget content visibility
widgetContent.slideToggle();
});
}
});