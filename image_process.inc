
<?php

// $Id$

/*
 *
 *
 * This Class implements the methods defined in the basic image solution content model
 */

class ImageProcessor {

  function ImageProcessor() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
  }

  //use imagemapi to manipulate images instead of going directly to imagemagick or whatever
  function manipulateImage($parameterArray = NULL, $dsid, $file, $file_ext) {
    $height = $parameterArray['height'];
    $width = $parameterArray['width'];

    $file_suffix = '_' . $dsid . '.' . $file_ext;
    $returnValue = TRUE;


    $image = imageapi_image_open($file);

    if (!$image) {
      drupal_set_message(t("Error opening image."));
      return FALSE;
    }

    if (!empty($height) || !empty($width)) {
      $returnValue = imageapi_image_scale($image, $height, $width);
    }

    if (!$returnValue) {
      drupal_set_message(t("Error scaling image."));
      return $returnValue;
    }
    $filename = substr(strrchr($file, '/'), 1);
    $output_path = $_SERVER['DOCUMENT_ROOT'] . base_path() . file_directory_path() . '/' . $filename . $file_suffix;
    $returnValue = imageapi_image_close($image, $output_path);
    if ($returnValue) {
      $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
      return TRUE;
    }
    else {
      return $returnValue;
    }
  }



}

