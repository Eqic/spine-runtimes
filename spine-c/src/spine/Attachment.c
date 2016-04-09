/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 * 
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/Attachment.h>
#include <spine/extension.h>
#include <spine/Slot.h>

#include <spine/MeshAttachment.h>
#include <spine/WeightedMeshAttachment.h>

typedef struct _spAttachmentVtable {
	void (*dispose) (spAttachment* self);
} _spAttachmentVtable;

void _spAttachment_init (spAttachment* self, const char* name, spAttachmentType type, /**/
		void (*dispose) (spAttachment* self)) {

	CONST_CAST(_spAttachmentVtable*, self->vtable) = NEW(_spAttachmentVtable);
	VTABLE(spAttachment, self) ->dispose = dispose;

	MALLOC_STR(self->name, name);
	CONST_CAST(spAttachmentType, self->type) = type;
}

void _spAttachment_deinit (spAttachment* self) {
	if (self->attachmentLoader) spAttachmentLoader_disposeAttachment(self->attachmentLoader, self);
	FREE(self->vtable);
	FREE(self->name);
}

void spAttachment_dispose (spAttachment* self) {
	VTABLE(spAttachment, self) ->dispose(self);
}


spRegionAttachment* spRegionAttachment_clone( spRegionAttachment* );
spBoundingBoxAttachment* spBoundingBoxAttachment_clone( spBoundingBoxAttachment* );
spMeshAttachment* spMeshAttachment_clone( spMeshAttachment* );
spWeightedMeshAttachment* spWeightedMeshAttachment_clone( spWeightedMeshAttachment* );

spAttachment* spAttachment_clone( spAttachment* source )
{
  spAttachment* self = 0;
  if ( source->type == SP_ATTACHMENT_REGION )
  {
    spRegionAttachment* copy = spRegionAttachment_clone( SUB_CAST( spRegionAttachment, source ) );
    self = &(copy->super);
  }
  else if ( source->type == SP_ATTACHMENT_BOUNDING_BOX )
  {
    spBoundingBoxAttachment* copy = spBoundingBoxAttachment_clone( SUB_CAST( spBoundingBoxAttachment, source ) );
    self = &(copy->super);
  }
  else if ( source->type == SP_ATTACHMENT_MESH )
  {
    spMeshAttachment* copy = spMeshAttachment_clone( SUB_CAST( spMeshAttachment, source ) );
    self = &(copy->super);
  }
  else if ( source->type == SP_ATTACHMENT_WEIGHTED_MESH )
  {
    spWeightedMeshAttachment* copy = spWeightedMeshAttachment_clone( SUB_CAST( spWeightedMeshAttachment, source ) );
    self = &(copy->super);
  }
  
  return self;
}

spRegionAttachment* spRegionAttachment_clone( spRegionAttachment* source )
{
  spRegionAttachment* self = spRegionAttachment_create( source->super.name );
  self->rendererObject = source->rendererObject;
  self->r = source->r;
  self->g = source->g;
  self->b = source->b;
  self->a = source->a;
  
  self->regionOffsetX         = source->regionOffsetX;
  self->regionOffsetY         = source->regionOffsetY;
  self->regionWidth           = source->regionWidth;
  self->regionHeight          = source->regionHeight;
  self->regionOriginalWidth   = source->regionOriginalWidth;
  self->regionOriginalHeight  = source->regionOriginalHeight;
  
  self->x         = source->x;
  self->y         = source->y;
  self->scaleX    = source->scaleX;
  self->scaleY    = source->scaleY;
  self->rotation  = source->rotation;
  self->width     = source->width;
  self->height    = source->height;
  
  // path
  //
  self->path = NULL;
  if ( source->path != NULL )
  {
    size_t sizeInBytes = sizeof( char ) * ( strlen( source->path ) + 1 );
    self->path = malloc( sizeInBytes );
    memcpy( (void*)self->path, source->path, sizeInBytes );
  }
  
  // offset, uvs
  //
  memcpy( self->offset, source->offset, ( sizeof( float ) * 8 ) );
  memcpy( self->uvs, source->uvs, ( sizeof( float ) * 8 ) );
  
  return self;
}

spBoundingBoxAttachment* spBoundingBoxAttachment_clone( spBoundingBoxAttachment* source )
{
  spBoundingBoxAttachment* self = spBoundingBoxAttachment_create( source->super.name );
  
  // vertices, verticesCount
  //
  self->verticesCount = source->verticesCount;
  self->vertices = NULL;
  if ( source->vertices != NULL )
  {
    self->vertices = MALLOC( float, source->verticesCount );
    memcpy( self->vertices, source->vertices, ( sizeof( float ) * source->verticesCount ) );
  }
  
  return self;
}

spMeshAttachment* spMeshAttachment_clone( spMeshAttachment* source )
{
  spMeshAttachment* self = spMeshAttachment_create( source->super.name );
  self->rendererObject = source->rendererObject;
  self->r = source->r;
  self->g = source->g;
  self->b = source->b;
  self->a = source->a;
  
  self->regionU = source->regionU;
  self->regionV = source->regionV;
  self->regionU2 = source->regionU2;
  self->regionV2 = source->regionV2;
  self->regionRotate = source->regionRotate;
  self->regionOffsetX = source->regionOffsetX;
  self->regionOffsetY = source->regionOffsetY;
  self->regionWidth = source->regionWidth;
  self->regionHeight = source->regionHeight;
  self->regionOriginalWidth = source->regionOriginalWidth;
  self->regionOriginalHeight = source->regionOriginalHeight;
  
  self->hullLength = source->hullLength;
  self->width   = source->width;
  self->height  = source->height;
  
  // path
  //
  self->path = NULL;
  if ( source->path != NULL )
  {
    size_t sizeInBytes = sizeof( char ) * ( strlen( source->path ) + 1 );
    self->path = malloc( sizeInBytes );
    memcpy( (void*)self->path, source->path, sizeInBytes );
  }
  
  // vertices, verticesCount
  //
  self->verticesCount = source->verticesCount;
  self->vertices = NULL;
  if ( source->vertices != NULL )
  {
    self->vertices = MALLOC( float, source->verticesCount );
    memcpy( self->vertices, source->vertices, ( sizeof( float ) * source->verticesCount ) );
  }
  
  // regionUVs, uvs
  //
  self->regionUVs = NULL;
  if ( source->regionUVs != NULL )
  {
    self->regionUVs = MALLOC( float, source->verticesCount );
    memcpy( self->regionUVs, source->regionUVs, ( sizeof( float ) * source->verticesCount ) );
  }
  
  self->uvs = NULL;
  if ( source->uvs != NULL )
  {
    self->uvs = MALLOC( float, source->verticesCount );
    memcpy( self->uvs, source->uvs, ( sizeof( float ) * source->verticesCount ) );
  }
  
  // triangles, trianglesCount
  //
  self->trianglesCount = source->trianglesCount;
  self->triangles = NULL;
  if ( source->triangles != NULL )
  {
    self->triangles = MALLOC( unsigned short, source->trianglesCount );
    memcpy( self->triangles, source->triangles, ( sizeof( int ) * source->trianglesCount ) );
  }
  
  // edges, edgesCount
  //
  self->edgesCount = source->edgesCount;
  self->edges = NULL;
  if ( source->edges != NULL )
  {
    self->edges = MALLOC( int, source->edgesCount );
    memcpy( self->edges, source->edges, ( sizeof( int ) * source->edgesCount ) );
  }
  
  return self;
}

spWeightedMeshAttachment* spWeightedMeshAttachment_clone( spWeightedMeshAttachment* source )
{
  spWeightedMeshAttachment* self = spWeightedMeshAttachment_create( source->super.name );
  self->rendererObject = source->rendererObject;
  self->r = source->r;
  self->g = source->g;
  self->b = source->b;
  self->a = source->a;
  
  self->regionU = source->regionU;
  self->regionV = source->regionV;
  self->regionU2 = source->regionU2;
  self->regionV2 = source->regionV2;
  self->regionRotate = source->regionRotate;
  self->regionOffsetX = source->regionOffsetX;
  self->regionOffsetY = source->regionOffsetY;
  self->regionWidth = source->regionWidth;
  self->regionHeight = source->regionHeight;
  self->regionOriginalWidth = source->regionOriginalWidth;
  self->regionOriginalHeight = source->regionOriginalHeight;
  
  self->hullLength = source->hullLength;
  self->width   = source->width;
  self->height  = source->height;
  
  // path
  //
  self->path = NULL;
  if ( source->path != NULL )
  {
    size_t sizeInBytes = sizeof( char ) * ( strlen( source->path ) + 1 );
    self->path = malloc( sizeInBytes );
    memcpy( (void*)self->path, source->path, sizeInBytes );
  }
  
  // bones, bonesCount
  //
  self->bonesCount = source->bonesCount;
  self->bones = NULL;
  if ( source->bones != NULL )
  {
    self->bones = MALLOC( int, source->bonesCount );
    memcpy( self->bones, source->bones, ( sizeof( int ) * source->bonesCount ) );
  }
  
  // weights, weightsCount
  //
  self->weightsCount = source->weightsCount;
  self->weights = NULL;
  if ( source->weights != NULL )
  {
    self->weights = MALLOC( float, source->weightsCount );
    memcpy( self->weights, source->weights, ( sizeof( float ) * source->weightsCount ) );
  }
  
  // triangles, trianglesCount
  //
  self->trianglesCount = source->trianglesCount;
  self->triangles = NULL;
  if ( source->triangles != NULL )
  {
    self->triangles = MALLOC( unsigned short, source->trianglesCount );
    memcpy( self->triangles, source->triangles, ( sizeof( int ) * source->trianglesCount ) );
  }
  
  // regionUVs, uvs, uvsCount
  //
  self->uvsCount = source->uvsCount;
  self->regionUVs = NULL;
  if ( source->regionUVs != NULL )
  {
    self->regionUVs = MALLOC( float, source->uvsCount );
    memcpy( self->regionUVs, source->regionUVs, ( sizeof( float ) * source->uvsCount ) );
  }
  
  self->uvs = NULL;
  if ( source->uvs != NULL )
  {
    self->uvs = MALLOC( float, source->uvsCount );
    memcpy( self->uvs, source->uvs, ( sizeof( float ) * source->uvsCount ) );
  }
  
  // edges, edgesCount
  //
  self->edgesCount = source->edgesCount;
  self->edges = NULL;
  if ( source->edges != NULL )
  {
    self->edges = MALLOC( int, source->edgesCount );
    memcpy( self->edges, source->edges, ( sizeof( int ) * source->edgesCount ) );
  }
  
  return self;
}
