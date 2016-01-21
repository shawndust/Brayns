/* Copyright (c) 2011-2015, EPFL/Blue Brain Project
 *                     Cyrille Favreau <cyrille.favreau@epfl.ch>
 *
 * This file is part of BRayns
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

#ifndef RENDERER_H
#define RENDERER_H

#include <brayns/api.h>
#include <brayns/common/parameters/RenderingParameters.h>

namespace brayns
{

class Renderer
{
public:
    BRAYNS_API Renderer(
        int argc, const char **argv,
        RenderingParametersPtr renderingParameters );
    BRAYNS_API virtual ~Renderer() {}

    BRAYNS_API virtual void render(FrameBufferPtr frameBuffer) = 0;

    BRAYNS_API virtual void commit() =  0;
    BRAYNS_API virtual void setScene(ScenePtr scene) =  0;
    BRAYNS_API virtual void setCamera(CameraPtr camera) =  0;

protected:
    RenderingParametersPtr _renderingParameters;
};

}
#endif // RENDERER_H