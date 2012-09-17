<?php

/**
 * @file
 * Image Processor and Display Image classes
 * @TODO: rename this file? There's two image_process.inc files in this module
 */

/**
 * This Class implements the methods defined in the basic image solution content model
 */
class ImageProcessor {

  /**
   * Constructor
   */
  function ImageProcessor() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
  }

  /**
   * use imagemapi to manipulate images instead of going directly to imagemagick or whatever
   * @param type $parameterArray
   * @param type $dsid
   * @param type $file
   * @param type $file_ext
   * @return type
   */
  function manipulateImage($parameterArray = NULL, $dsid, $file, $file_ext) {
    $height = $parameterArray['height'];
    $width = $parameterArray['width'];

    $file_suffix = '_' . $dsid . '.' . $file_ext;
    $returnValue = TRUE;

    $path_info = pathinfo($file);
    $newfile = str_replace('.' . $path_info['extension'], '_EXIF.xml', $file);
    $returnValue = TRUE;
    $output = array();
    exec('exiftool -X ' . escapeshellarg($file) . '', $output, $returnValue);
    if ($returnValue == 0) {
      file_put_contents($newfile, implode("\n", $output));
    }

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
    $filename = drupal_substr(strrchr($file, '/'), 1);
    $output_path = $_SERVER['DOCUMENT_ROOT'] . base_path() . $file . $file_suffix;
    $returnValue = imageapi_image_close($image, $output_path);

    return $returnValue;
  }

}

/**
 * Display Image Streams
 */
class DisplayImageStreams {

  private $pid = NULL;

  /**
   * Constructor
   * @param type $pid
   */
  function DisplayImageStreams($pid) {
    //drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $this->pid = $pid;
  }

  /**
   * Show medium size
   * @global type $base_url
   * @return type
   */
  function showMediumSize() {
    global $base_url;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new fedora_item($this->pid);
    $title = $item->objectProfile->objLabel;
    $collection_fieldset = array(
      '#collapsible' => FALSE,
      '#value' => '<div><div><a href="' . $base_url . '/fedora/repository/' . $this->pid . '/OBJ/FULL_SIZE.jpg">View Full Size</a></div>' .
      '<div><a href="' . $base_url . '/fedora/repository/' . $this->pid . '/OBJ/FULL_SIZE.jpg"><img src="' .
      $base_url . '/fedora/repository/' . $this->pid . '/MEDIUM_SIZE/MEDIUM_SIZE' . '" /></a></div>',
    );
    $tabset = array();
    $tabset['image'] = array(
      '#type' => 'tabpage',
      '#title' => $title,
      '#content' => theme('fieldset', $collection_fieldset),
    );

    return $tabset;
    //  return theme('fieldset', $collection_fieldset);
  }

  /**
   * Show thumbnail
   * @global type $base_url
   * @return type
   */
  function showTN() {
    global $base_url;
    $collection_fieldset = array(
      '#collapsible' => FALSE,
      '#value' => '<a href="' . $base_url . '/fedora/repository/' . $this->pid . '/MEDIUM_SIZE/MEDIUM_SIZE.jpg"><img src="' .
      $base_url . '/fedora/repository/' . $this->pid . '/TN/TN.jpg' . '" /></a>',
    );
    return theme('fieldset', $collection_fieldset);
  }
}
