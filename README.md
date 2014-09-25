# Basic Image Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_image.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_image)

## Introduction

Creates Image Collection to hold image objects. Adds support for image objects based on JPEG/PNG/GIF image files.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [ImageMagick](https://drupal.org/project/imagemagick)

*To successfully create derivative data streams ImageMagick (for TN & JPG) needs to be installed on the server.*

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the image-tool kit to use ImageMagick rather than GD in Administration > Configuration > Media > Image Toolkit (admin/config/media/image-toolkit). If GD is selected, TN and JPG datastreams will not be generated.

![Configuration](http://i.imgur.com/O3sQPeO.png)

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors
Current maintainers:

* [Alan Stanley](https://github.com/ajstanley)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
