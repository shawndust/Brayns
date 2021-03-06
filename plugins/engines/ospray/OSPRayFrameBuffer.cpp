/* Copyright (c) 2015-2016, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Cyrille Favreau <cyrille.favreau@epfl.ch>
 *
 * This file is part of Brayns <https://github.com/BlueBrain/Brayns>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "OSPRayFrameBuffer.h"

#include <brayns/common/log.h>
#include <ospray/SDK/common/OSPCommon.h>

namespace brayns
{
OSPRayFrameBuffer::OSPRayFrameBuffer(const Vector2ui& frameSize,
                                     const FrameBufferFormat colorDepth,
                                     const bool accumulation)
    : FrameBuffer(frameSize, colorDepth, accumulation)
    , _frameBuffer(0)
    , _colorBuffer(0)
    , _depthBuffer(0)
{
    resize(frameSize);
}

OSPRayFrameBuffer::~OSPRayFrameBuffer()
{
    unmap();
    ospFreeFrameBuffer(_frameBuffer);
}

void OSPRayFrameBuffer::resize(const Vector2ui& frameSize)
{
    _frameSize = frameSize;

    if (_frameBuffer)
    {
        unmap();
        ospFreeFrameBuffer(_frameBuffer);
    }

    OSPFrameBufferFormat format;
    switch (_frameBufferFormat)
    {
    case FBF_RGBA_I8:
        format = OSP_FB_RGBA8;
        break;
    case FBF_RGBA_F32:
        format = OSP_FB_RGBA32F;
        break;
    default:
        format = OSP_FB_NONE;
    }

    osp::vec2i size = {_frameSize.x(), _frameSize.y()};

    size_t attributes = OSP_FB_COLOR | OSP_FB_DEPTH;
    if (_accumulation)
        attributes |= OSP_FB_ACCUM;

    _frameBuffer = ospNewFrameBuffer(size, format, attributes);
    ospCommit(_frameBuffer);
    clear();
}

void OSPRayFrameBuffer::clear()
{
    size_t attributes = 0;
    if (_accumulation)
        attributes |= OSP_FB_ACCUM;
    ospFrameBufferClear(_frameBuffer, attributes);
}

void OSPRayFrameBuffer::map()
{
    _colorBuffer = (uint8_t*)ospMapFrameBuffer(_frameBuffer, OSP_FB_COLOR);
    _depthBuffer = (float*)ospMapFrameBuffer(_frameBuffer, OSP_FB_DEPTH);
}

void OSPRayFrameBuffer::unmap()
{
    if (_colorBuffer)
    {
        ospUnmapFrameBuffer(_colorBuffer, _frameBuffer);
        _colorBuffer = 0;
    }

    if (_depthBuffer)
    {
        ospUnmapFrameBuffer(_depthBuffer, _frameBuffer);
        _depthBuffer = 0;
    }
}
}
