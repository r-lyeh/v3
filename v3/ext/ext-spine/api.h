#if !CODE

//#include "inc/spine/spine.h"

#include "inc/spine/dll.h"

#include "inc/spine/Animation.h"
#include "inc/spine/AnimationState.h"
#include "inc/spine/AnimationStateData.h"
#include "inc/spine/Array.h"
#include "inc/spine/Atlas.h"
#include "inc/spine/AtlasAttachmentLoader.h"
#include "inc/spine/Attachment.h"
#include "inc/spine/AttachmentLoader.h"
#include "inc/spine/Bone.h"
#include "inc/spine/BoneData.h"
#include "inc/spine/BoundingBoxAttachment.h"
#include "inc/spine/ClippingAttachment.h"
#include "inc/spine/Color.h"
#include "inc/spine/Debug.h"
#include "inc/spine/Event.h"
#include "inc/spine/EventData.h"
#include "inc/spine/extension.h"
#include "inc/spine/IkConstraint.h"
#include "inc/spine/IkConstraintData.h"
#include "inc/spine/MeshAttachment.h"
#include "inc/spine/PathAttachment.h"
#include "inc/spine/PathConstraint.h"
#include "inc/spine/PathConstraintData.h"
#include "inc/spine/Physics.h"
#include "inc/spine/PhysicsConstraint.h"
#include "inc/spine/PhysicsConstraintData.h"
#include "inc/spine/PointAttachment.h"
#include "inc/spine/RegionAttachment.h"
#include "inc/spine/Sequence.h"
#include "inc/spine/Skeleton.h"
#include "inc/spine/SkeletonBinary.h"
#include "inc/spine/SkeletonBounds.h"
#include "inc/spine/SkeletonClipping.h"
#include "inc/spine/SkeletonData.h"
#include "inc/spine/SkeletonJson.h"
#include "inc/spine/Skin.h"
#include "inc/spine/Slot.h"
#include "inc/spine/SlotData.h"
#include "inc/spine/TextureRegion.h"
#include "inc/spine/TransformConstraint.h"
#include "inc/spine/TransformConstraintData.h"
#include "inc/spine/Triangulator.h"
#include "inc/spine/Version.h"
#include "inc/spine/VertexAttachment.h"

#if 0
#include <spine/dll.h>
#include <spine/Array.h>
#include <spine/Animation.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>
#include <spine/Atlas.h>
#include <spine/AtlasAttachmentLoader.h>
#include <spine/Attachment.h>
#include <spine/AttachmentLoader.h>
#include <spine/Bone.h>
#include <spine/BoneData.h>
#include <spine/RegionAttachment.h>
#include <spine/VertexAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/BoundingBoxAttachment.h>
#include <spine/ClippingAttachment.h>
#include <spine/Physics.h>
#include <spine/PointAttachment.h>
#include <spine/Skeleton.h>
#include <spine/SkeletonBounds.h>
#include <spine/SkeletonData.h>
#include <spine/SkeletonBinary.h>
#include <spine/SkeletonJson.h>
#include <spine/Skin.h>
#include <spine/Slot.h>
#include <spine/SlotData.h>
#include <spine/SkeletonClipping.h>
#include <spine/Event.h>
#include <spine/EventData.h>
#endif

#elif CODE
#pragma once

#pragma push_macro("listener")
#undef  listener

#define skip skip_B // stb_vorbis

#include "src/Animation.c"
#include "src/AnimationState.c"
#include "src/AnimationStateData.c"
#include "src/Array.c"
#include "src/Atlas.c"
#include "src/AtlasAttachmentLoader.c"
#include "src/Attachment.c"
#include "src/AttachmentLoader.c"
#include "src/Bone.c"
#include "src/BoneData.c"
#include "src/BoundingBoxAttachment.c"
#include "src/ClippingAttachment.c"
#include "src/Color.c"
#include "src/Debug.c"
#include "src/Event.c"
#include "src/EventData.c"
#include "src/extension.c"
#include "src/IkConstraint.c"
#include "src/IkConstraintData.c"
#include "src/Json.c"
#include "src/MeshAttachment.c"
#include "src/PathAttachment.c"
#include "src/PathConstraint.c"
#include "src/PathConstraintData.c"
#include "src/PhysicsConstraint.c"
#include "src/PhysicsConstraintData.c"
#include "src/PointAttachment.c"
#include "src/RegionAttachment.c"
#include "src/Sequence.c"
#include "src/SkeletonJson.c"
#include "src/Skeleton.c"
#include "src/SkeletonBounds.c"
#include "src/SkeletonClipping.c"
#include "src/SkeletonData.c"
#include "src/Skin.c"
#include "src/Slot.c"
#include "src/SlotData.c"
#include "src/TransformConstraint.c"
#include "src/TransformConstraintData.c"
#include "src/Triangulator.c"
#include "src/VertexAttachment.c"

//#include <spine/SkeletonBinary.h>
#define _spLinkedMesh _spLinkedMesh_B
#define _readVertices _readVertices_B
#define cleanUpTimelines cleanUpTimelines_B
#define findSlotIndex findSlotIndex_B
#define readCurve readCurve_B
#define readSequence readSequence_B
#define readTimeline readTimeline_B
#define readTimeline2 readTimeline2_B
#define setBezier setBezier_B
#define string_starts_with string_starts_with_B
#define toColor toColor_B
#define toColor2 toColor2_B
#include "src/SkeletonBinary.c"

#pragma pop_macro("listener")

#endif
