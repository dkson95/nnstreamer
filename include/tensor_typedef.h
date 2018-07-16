/**
 * NNStreamer Common Header, Typedef part, for export as devel package.
 * Copyright (C) 2018 MyungJoo Ham <myungjoo.ham@samsung.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 */
/**
 * @file	tensor_common_typedef.h
 * @date	01 Jun 2018
 * @brief	Common header file for NNStreamer, the GStreamer plugin for neural networks
 * @see		http://github.com/TO-BE-DETERMINED-SOON
 * @see		https://github.sec.samsung.net/STAR/nnstreamer
 * @author	MyungJoo Ham <myungjoo.ham@samsung.com>
 * @bug		No known bugs except for NYI items
 *
 * To Packagers:
 *
 * This fils it to be packaged as "devel" package for NN developers.
 */
#ifndef __GST_TENSOR_TYPEDEF_H__
#define __GST_TENSOR_TYPEDEF_H__

#define NNS_TENSOR_RANK_LIMIT	(4)
#define NNS_TENSOR_DIM_NULL ({0, 0, 0, 0})
/**
 * @brief Possible data element types of other/tensor.
 *
 * The current version supports NNS_UINT8 only as video-input.
 * There is no restrictions for inter-NN or sink-to-app.
 */
typedef enum _nns_tensor_type {
  _NNS_INT32 = 0,
  _NNS_UINT32,
  _NNS_INT16,
  _NNS_UINT16,
  _NNS_INT8,
  _NNS_UINT8,
  _NNS_FLOAT64,
  _NNS_FLOAT32,

  _NNS_END,
} tensor_type;

/**
 * @brief Byte-per-element of each tensor element type.
 */
static const unsigned int tensor_element_size[] = {
        [_NNS_INT32] = 4,
        [_NNS_UINT32] = 4,
        [_NNS_INT16] = 2,
        [_NNS_UINT16] = 2,
        [_NNS_INT8] = 1,
        [_NNS_UINT8] = 1,
        [_NNS_FLOAT64] = 8,
        [_NNS_FLOAT32] = 4,
};

/**
 * @brief NN Frameworks available for the tensor_filter element.
 */
typedef enum _nnfw_type {
  _T_F_UNDEFINED = 0, /**< Not defined or supported. Cannot proceed in this status */

  _T_F_CUSTOM, /**< Custom filter provided as a shared object (dysym) */
  _T_F_TENSORFLOW_LITE, /**< In Progress */
  _T_F_TENSORFLOW, /**< NYI */
  _T_F_CAFFE2, /**< NYI */

  _T_F_NNFW_END,
} nnfw_type;

struct _GstTensor_Filter_Framework;
typedef struct _GstTensor_Filter_Framework GstTensor_Filter_Framework;

typedef enum {
  _TFC_INIT = 0,
  _TFC_DIMENSION = 1,
  _TFC_TYPE = 2,
  _TFC_ALL = _TFC_DIMENSION | _TFC_TYPE,

  _TFC_FRAMERATE = 4,

  /** @todo Add "consistency checked. don't check it again" and implement .c accordingly. */
} GstTensor_Filter_CheckStatus;

typedef uint32_t tensor_dim[NNS_TENSOR_RANK_LIMIT];

/**
 * @brief Tensor_Filter's properties (internal data structure)
 *
 * Because custom filters of tensor_filter may need to access internal data
 * of Tensor_Filter, we define this data structure here.
 */
typedef struct _GstTensor_Filter_Properties
{
  int silent; /**< Verbose mode if FALSE. int instead of gboolean for non-glib custom plugins */
  GstTensor_Filter_CheckStatus inputConfigured; /**< input dimension status */
  GstTensor_Filter_CheckStatus outputConfigured; /**< output dimension status */
  nnfw_type nnfw; /**< The enum value of corresponding NNFW. _T_F_UNDEFINED if not configured */
  GstTensor_Filter_Framework *fw; /**< The implementation core of the NNFW. NULL if not configured */
  int fwOpened; /**< true IF open() is called or tried. Use int instead of gboolean because this is refered by custom plugins. */
  int fwClosed; /**< true IF close() is called or tried. Use int instead of gboolean because this is refered by custom plugins. */
  const char *modelFilename; /**< Filepath to the model file (as an argument for NNFW). char instead of gchar for non-glib custom plugins */

  tensor_dim inputDimension; /**< The input tensor dimension */
  tensor_type inputType; /**< The type for each element in the input tensor */
  int inputCapNegotiated; /**< @todo check if this is really needed */
  tensor_dim outputDimension; /**< The output tensor dimension */
  tensor_type outputType; /**< The type for each element in the output tensor */
  int outputCapNegotiated; /**< @todo check if this is really needed */

  const char *customProperties; /**< sub-plugin specific custom property values in string */
} GstTensor_Filter_Properties;

#endif /*__GST_TENSOR_TYPEDEF_H__*/
