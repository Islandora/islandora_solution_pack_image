<?php

/**
 * @file
 * Functions deriving images for objects.
 */

/**
 * Scales the given image.
 *
 * @param object $file
 *   The image file to scale.
 * @param int $width
 *   The width to scale the derived image to.
 * @param int $height
 *   The height to scale the derived image to.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_basic_image_scale_image($file, $width, $height) {
  $real_path = drupal_realpath($file->uri);
  $image = image_load($real_path);
  if (!empty($image)) {
    $scale = image_scale($image, $width, $height, TRUE);
    if ($scale) {
      return image_save($image);
    }
  }
  return FALSE;
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file
 *   The URI of the file to be added as the datastream content.
 */
function islandora_basic_image_add_datastream(AbstractObject $object, $dsid, $file) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = $object['OBJ']->mimeType;
    $ds->setContentFromFile(drupal_realpath($file->uri));
    $object->ingestDatastream($ds);
    file_delete($file);
    return TRUE;
  }
  catch (exception $e) {
    $message = $e->getMessage();
    return $message;
  }
}

function islandora_basic_image_create_thumbnail(AbstractObject $object, $force = FALSE) {
  $return = '';
  if (!isset($object['TN']) && $force === FALSE) {
    $original_file = islandora_basic_image_copy_obj_datastream($object);
    if ($original_file === FALSE) {
      return islandora_basic_image_no_obj_datastream($object->id);
    }
    else {
      $tn_file = file_copy($original_file['file'], 'temporary://' . $original_file['filename'] . 'TN.' . $original_file['extension']);
      if (islandora_basic_image_scale_image($tn_file, 200, 200)) {
        $added_successfully = islandora_basic_image_add_datastream($object, 'TN', $tn_file);
        if ($added_successfully !== TRUE) {
          $return = islandora_basic_image_failed_add($added_successfully);
        }
        else {
          $return = array(
            'success' => TRUE,
            'messages' => array(
              array(
                'message' => t('The TN datastream was added successfully for @pid!', array('@pid' => $object->id)),
                'type' => 'info',
              ),
            ),
          );
        }
      }
      else {
        $return = islandora_basic_image_failed_scaling();
      }
    }
    if (isset($original_file)) {
      file_delete($original_file['file']);
    }
    return $return;
  }
}


/**
 * Creates the derivatives for this content model type.
 *
 * @param AbstractObject $object
 *   The object to add derivatives to.
 */
function islandora_basic_image_create_medium_size(AbstractObject $object, $force = FALSE) {
  $return = '';
  if (!isset($object['MEDIUM_SIZE']) && $force === FALSE) {
    $original_file = islandora_basic_image_copy_obj_datastream($object);
    if ($original_file === FALSE) {
      return islandora_basic_image_no_obj_datastream($object->id);
    }
    else {
      $medium_file = file_copy($original_file['file'], 'temporary://' . $original_file['filename'] . 'TN.' . $original_file['extension']);
      if (islandora_basic_image_scale_image($medium_file, 500, 700)) {
        $added_successfully = islandora_basic_image_add_datastream($object, 'MEDIUM_SIZE', $medium_file);
        if ($added_successfully !== TRUE) {
         $return = islandora_basic_image_failed_add($added_successfully);
        }
        else {
          $return = array(
            'success' => TRUE,
            'messages' => array(
              array(
                'message' => t('The MEDIUM_SIZE datastream was added successfully for @pid!', array('@pid' => $object->id)),
                'type' => 'info',
              ),
            ),
          );
        }
      }
      else {
        $return = islandora_basic_image_failed_scaling();
      }
    }
    if (isset($original_file)) {
      file_delete($original_file['file']);
    }
    return $return;
  }
}

/**
 * Copies the OBJ to the file system.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @return array|stdClass
 *   FALSE when the object doesn't have the OBJ, an array otherwise.
 */
function islandora_basic_image_copy_obj_datastream(AbstractObject $object) {
  $mime_detect = new MimeDetect();
  if (!isset($object['OBJ'])) {
    return FALSE;
  }
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object we can save.
  $file_uri = file_create_filename("{$file_name}OBJ.{$ext}", 'temporary://');
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['OBJ']->mimeType;
  // Temporary.
  $file->status = 0;
  $object['OBJ']->getContent($file_uri);
  $original_file = file_save($file);
  return array(
    'file' => $original_file,
    'extension' => $ext,
    'filename' => $file_name,
  );
}

function islandora_basic_image_no_obj_datastream($pid) {
  return array(
    'success' => FALSE,
    'messages' => array(
      array(
        'message' => t('Could not create image derivatives for %s. No image file was uploaded.', array('%s' => $pid)),
        'type' => 'error',
      ),
    ),
  );
}

/**
 * Basic image fails to scale.
 *
 * @param string $message
 *   An error message to pass along.
 *
 * @return array
 *   An array in the format used by islandora_derivative().
 */
function islandora_basic_image_failed_scaling() {
  return array(
    'success' => FALSE,
    'messages' => array(
      array(
        'message' => t('Unable to scale the image, the datastream was not added.'),
        'type' => 'error',
      ),
    ),
  );
}

function islandora_basic_image_failed_adding($message) {
  return array(
    'success' => FALSE,
    'messages' => array(
      array(
        'message' => $message,
        'type' => 'error',
      ),
    ),
  );
}