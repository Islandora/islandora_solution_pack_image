<?php
/**
 * @file image.process.inc
 *
 * This file contains all the functions for image manipulation used in the
 * basic image solution pack.
 */

/**
 * creates the derivatives for this content model type
 * @param object $object
 *   tuque FedoraObject
 * @return string 
 */
function islandora_basic_image_create_all_derivatives($files, $object) {
  module_load_include('inc', 'islandora', 'includes/mime.detect');
  module_load_include('inc', 'islandora_basic_image', 'includes/image_process');
  if (!isset($files['OBJ'])) {
    drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.', array('%s' => $object->id)),'error');
    return "";
  }

  $original_file = $files["OBJ"];
  $info = pathinfo($original_file);
  $ext = $info["extension"];
  $file_name = $info["filename"];

  $tn_file = file_unmanaged_copy($original_file, 'temporary://' . $file_name . 'TN.' . $ext);
  $_SESSION["fedora_ingest_files"]["TN"] = $tn_file;
  if (islandora_basic_image_create_derivative($tn_file, 200, 200)) {
    $files["TN"] = $tn_file;
  }

  $medium_file = file_unmanaged_copy($original_file, 'temporary://' . $file_name . 'MEDIUM.' . $ext);
  $_SESSION["fedora_ingest_files"]["MEDIUM_SIZE"] = $medium_file;
  if (islandora_basic_image_create_derivative($medium_file, 500, 700)) {
    $files["MEDIUM_SIZE"] = $medium_file;
  }

  return $files;
}


/**
 *
 * @param object $file 
 *   stdclass
 */
function islandora_basic_image_create_derivative($file, $width, $height) {
  $real_path = drupal_realpath($file);
  $image = image_load($real_path);
  if (!empty($image)) {
    $scale = image_scale($image, $width, $height, TRUE);
    if ($scale) {
      return image_save($image);
    }
  }
  return FALSE;
}
