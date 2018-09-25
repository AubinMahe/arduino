<?php

$path = '/home/aubin/Dev/Git';
require_once $path . '/minify/src/Minify.php';
require_once $path . '/minify/src/CSS.php';
require_once $path . '/minify/src/JS.php';
require_once $path . '/minify/src/Exception.php';
require_once $path . '/minify/src/Exceptions/BasicException.php';
require_once $path . '/minify/src/Exceptions/FileImportException.php';
require_once $path . '/minify/src/Exceptions/IOException.php';
require_once $path . '/path-converter/src/ConverterInterface.php';
require_once $path . '/path-converter/src/Converter.php';

use MatthiasMullie\Minify;

$m = new Minify\CSS( 'www/arrosage.css' );
$m->gzip( 'www/arrosage-min.css.gz' );

$m = new Minify\JS ( 'www/arrosage.js'  );
$m->gzip( 'www/arrosage-min.js.gz' );

?>
