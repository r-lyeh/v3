#if !CODE

//#include "3rd/spine/spine.h"

#include "3rd/spine/dll.h"

#include "3rd/spine/Animation.h"
#include "3rd/spine/AnimationState.h"
#include "3rd/spine/AnimationStateData.h"
#include "3rd/spine/Array.h"
#include "3rd/spine/Atlas.h"
#include "3rd/spine/AtlasAttachmentLoader.h"
#include "3rd/spine/Attachment.h"
#include "3rd/spine/AttachmentLoader.h"
#include "3rd/spine/Bone.h"
#include "3rd/spine/BoneData.h"
#include "3rd/spine/BoundingBoxAttachment.h"
#include "3rd/spine/ClippingAttachment.h"
#include "3rd/spine/Color.h"
#include "3rd/spine/Debug.h"
#include "3rd/spine/Event.h"
#include "3rd/spine/EventData.h"
#include "3rd/spine/IkConstraint.h"
#include "3rd/spine/IkConstraintData.h"
#include "3rd/spine/MeshAttachment.h"
#include "3rd/spine/PathAttachment.h"
#include "3rd/spine/PathConstraint.h"
#include "3rd/spine/PathConstraintData.h"
#include "3rd/spine/Physics.h"
#include "3rd/spine/PhysicsConstraint.h"
#include "3rd/spine/PhysicsConstraintData.h"
#include "3rd/spine/PointAttachment.h"
#include "3rd/spine/RegionAttachment.h"
#include "3rd/spine/Sequence.h"
#include "3rd/spine/Skeleton.h"
#include "3rd/spine/SkeletonBinary.h"
#include "3rd/spine/SkeletonBounds.h"
#include "3rd/spine/SkeletonClipping.h"
#include "3rd/spine/SkeletonData.h"
#include "3rd/spine/SkeletonJson.h"
#include "3rd/spine/Skin.h"
#include "3rd/spine/Slot.h"
#include "3rd/spine/SlotData.h"
#include "3rd/spine/TextureRegion.h"
#include "3rd/spine/TransformConstraint.h"
#include "3rd/spine/TransformConstraintData.h"
#include "3rd/spine/Triangulator.h"
#include "3rd/spine/Version.h"
#include "3rd/spine/VertexAttachment.h"

#elif CODE
#pragma once

#pragma push_macro("listener")
#undef  listener

#pragma push_macro("PI") // lua
#undef PI

#pragma push_macro("UNUSED") // lua
#undef UNUSED

#define skip skip_SPINE // stb_vorbis

#include "3rd/spine/extension.h"

#include "3rd/src/Animation.c"
#include "3rd/src/AnimationState.c"
#include "3rd/src/AnimationStateData.c"
#include "3rd/src/Array.c"
#include "3rd/src/Atlas.c"
#include "3rd/src/AtlasAttachmentLoader.c"
#include "3rd/src/Attachment.c"
#include "3rd/src/AttachmentLoader.c"
#include "3rd/src/Bone.c"
#include "3rd/src/BoneData.c"
#include "3rd/src/BoundingBoxAttachment.c"
#include "3rd/src/ClippingAttachment.c"
#include "3rd/src/Color.c"
#include "3rd/src/Debug.c"
#include "3rd/src/Event.c"
#include "3rd/src/EventData.c"
#include "3rd/src/extension.c"
#include "3rd/src/IkConstraint.c"
#include "3rd/src/IkConstraintData.c"
#include "3rd/src/Json.c"
#include "3rd/src/MeshAttachment.c"
#include "3rd/src/PathAttachment.c"
#include "3rd/src/PathConstraint.c"
#include "3rd/src/PathConstraintData.c"
#include "3rd/src/PhysicsConstraint.c"
#include "3rd/src/PhysicsConstraintData.c"
#include "3rd/src/PointAttachment.c"
#include "3rd/src/RegionAttachment.c"
#include "3rd/src/Sequence.c"
#include "3rd/src/SkeletonJson.c"
#include "3rd/src/Skeleton.c"
#include "3rd/src/SkeletonBounds.c"
#include "3rd/src/SkeletonClipping.c"
#include "3rd/src/SkeletonData.c"
#include "3rd/src/Skin.c"
#include "3rd/src/Slot.c"
#include "3rd/src/SlotData.c"
#include "3rd/src/TransformConstraint.c"
#include "3rd/src/TransformConstraintData.c"
#include "3rd/src/Triangulator.c"
#include "3rd/src/VertexAttachment.c"
#undef skip

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
#include "3rd/src/SkeletonBinary.c"

#pragma pop_macro("listener")
#pragma pop_macro("PI")
#pragma pop_macro("UNUSED")

#undef ABS // osx
#undef REALLOC // 

#endif
