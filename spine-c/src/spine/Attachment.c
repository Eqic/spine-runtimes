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
#include <spine/SkinnedMeshAttachment.h>

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
	FREE(self->vtable);
	FREE(self->name);
}

void spAttachment_dispose (spAttachment* self) {
	VTABLE(spAttachment, self) ->dispose(self);
}


spRegionAttachment* spRegionAttachment_clone( spRegionAttachment* );
spBoundingBoxAttachment* spBoundingBoxAttachment_clone( spBoundingBoxAttachment* );
spMeshAttachment* spMeshAttachment_clone( spMeshAttachment* );
spSkinnedMeshAttachment* spSkinnedMeshAttachment_clone( spSkinnedMeshAttachment* );

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
  else if ( source->type == SP_ATTACHMENT_SKINNED_MESH )
  {
    spSkinnedMeshAttachment* copy = spSkinnedMeshAttachment_clone( SUB_CAST( spSkinnedMeshAttachment, source ) );
    self = &(copy->super);
  }
  
  return self;
}

spRegionAttachment* spRegionAttachment_clone( spRegionAttachment* source )
{
  spRegionAttachment* self = spRegionAttachment_create( source->super.name );
  
  MALLOC_STR( self->path, source->path );
  self->rendererObject = source->rendererObject;
  
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
  
  self->r = source->r;
  self->g = source->g;
  self->b = source->b;
  self->a = source->a;
  
  int i;
  for ( i = 0; i < 8; ++i )
  {
    self->offset[ i ] = source->offset[ i ];
    self->uvs[ i ]    = source->uvs[ i ];
  }
  
  return self;
}

spBoundingBoxAttachment* spBoundingBoxAttachment_clone( spBoundingBoxAttachment* source )
{
  spBoundingBoxAttachment* self = spBoundingBoxAttachment_create( source->super.name );
  return self;
}

spMeshAttachment* spMeshAttachment_clone( spMeshAttachment* source )
{
  spMeshAttachment* attachment = spMeshAttachment_create( source->super.name );
  attachment->rendererObject = source->rendererObject;
  attachment->regionU = source->regionU;
  attachment->regionV = source->regionV;
  attachment->regionU2 = source->regionU2;
  attachment->regionV2 = source->regionV2;
  attachment->regionRotate = source->regionRotate;
  attachment->regionOffsetX = source->regionOffsetX;
  attachment->regionOffsetY = source->regionOffsetY;
  attachment->regionWidth = source->regionWidth;
  attachment->regionHeight = source->regionHeight;
  attachment->regionOriginalWidth = source->regionOriginalWidth;
  attachment->regionOriginalHeight = source->regionOriginalHeight;
  
  if ( source->path != NULL )
  {
    size_t sizeInBytes = sizeof( char ) * ( strlen( source->path ) + 1 );
    attachment->path = malloc( sizeInBytes );
    memcpy( (void*)attachment->path, source->path, sizeInBytes );
  }
  
  return attachment;
  
//  self->vertices = NULL;
//  self->regionUVs = NULL;
//  self->uvs = NULL;
//  self->triangles = NULL;
//  self->edges = NULL;
}

spSkinnedMeshAttachment* spSkinnedMeshAttachment_clone( spSkinnedMeshAttachment* source )
{
  spSkinnedMeshAttachment* attachment = spSkinnedMeshAttachment_create( source->super.name );
  attachment->rendererObject = source->rendererObject;
  attachment->regionU = source->regionU;
  attachment->regionV = source->regionV;
  attachment->regionU2 = source->regionU2;
  attachment->regionV2 = source->regionV2;
  attachment->regionRotate = source->regionRotate;
  attachment->regionOffsetX = source->regionOffsetX;
  attachment->regionOffsetY = source->regionOffsetY;
  attachment->regionWidth = source->regionWidth;
  attachment->regionHeight = source->regionHeight;
  attachment->regionOriginalWidth = source->regionOriginalWidth;
  attachment->regionOriginalHeight = source->regionOriginalHeight;
  
  if ( source->path != NULL )
  {
    size_t sizeInBytes = sizeof( char ) * ( strlen( source->path ) + 1 );
    attachment->path = malloc( sizeInBytes );
    memcpy( (void*)attachment->path, source->path, sizeInBytes );
  }
  
  return attachment;
  
//  self->bones = NULL;
//  self->weights = NULL;
//  self->triangles = NULL;
//  self->regionUVs = NULL;
//  self->uvs = NULL;
//  self->edges = NULL;
}
