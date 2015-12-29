/* From SO user Plippie */
function getParameterByName( name ) {
    var regexS = "[\\?&]"+name+"=([^&#]*)", 
    regex = new RegExp( regexS ),
    results = regex.exec( window.location.search );
    if( results == null ){
        return "";
    }
    return decodeURIComponent(results[1].replace(/\+/g, " "));
}

var color =getParameterByName('color');
if (color) {
    $('input[name=color]').val(color);
} else {
    console.log("Color not given, falling back to default\n");
}


