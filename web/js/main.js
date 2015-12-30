
/* Color Picker with Preview
 *
 * RGB to Hex: http://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
 */
function rawHex(r, g, b) {
    return ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1);
}
function rgbToHex(r, g, b) {
    return "#" + rawHex(r, g, b);
}

function updateColor() {
    var color = rgbToHex(
        Number($('input[name=r]').val()),
        Number($('input[name=g]').val()),
        Number($('input[name=b]').val())
    );

    $('#color-preview').css('background-color', color);
}
updateColor();

$('input[type=range]').change(function() {
    updateColor();
});


function sendColor() {
    $.post("http://newpi.local:8080/", "color=%23" + rawHex(
        Number($('input[name=r]').val()),
        Number($('input[name=g]').val()),
        Number($('input[name=b]').val())
    ));
    return false;
}

