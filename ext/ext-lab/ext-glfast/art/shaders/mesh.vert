#version 430 core
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_explicit_uniform_location : enable
out gl_PerVertex { vec4 gl_Position; };

vec4 qconj(vec4 q)
{
  return vec4(-q.xyz, q.w);
}

vec3 qrot(vec3 v, vec4 q)
{
  return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + v * q.w);
}

vec4 proj(vec3 mv, vec4 p)
{
  return vec4(mv.xy * p.xy, mv.z * p.z + p.w, -mv.z);
}

layout(location = 0) uniform vec3 cam_pos;
layout(location = 1) uniform vec4 cam_rot;
layout(location = 2) uniform vec4 cam_prj;

layout(std430, binding = 0) readonly buffer in_mesh_id_ { int in_mesh_id[]; };
layout(std430, binding = 1) readonly buffer in_attr_first_ { ivec4 in_attr_first[]; };
layout(std430, binding = 2) readonly buffer in_attr_id_ { ivec4 in_attr_id[]; };

layout(std430, binding = 3) readonly buffer in_pos_ { vec4 in_pos[]; };
layout(std430, binding = 4) readonly buffer in_uv_ { vec2 in_uv[]; };
layout(std430, binding = 5) readonly buffer in_normal_ { vec4 in_normal[]; };

layout(std430, binding = 6) readonly buffer in_ins_first_ { int in_ins_first[]; };
layout(std430, binding = 7) readonly buffer in_ins_pos_ { vec4 in_ins_pos[]; };

flat   out int  vs_id;
smooth out vec2 vs_uv;
smooth out vec3 vs_normal;
smooth out vec3 vs_position;

void main()
{
  int   mesh_id    = in_mesh_id[gl_VertexID];
  ivec3 attr_first = in_attr_first[mesh_id].xyz;
  ivec3 attr_id    = in_attr_id[gl_VertexID].xyz;
  
  vec3 pos    = in_pos[attr_first.x + attr_id.x].xyz;
  vec2 uv     = in_uv[attr_first.y + attr_id.y].xy;
  vec3 normal = in_normal[attr_first.z + attr_id.z].xyz;
  
  int  ins_first = in_ins_first[mesh_id];
  vec3 ins_pos   = in_ins_pos[ins_first + gl_InstanceID].xyz;
  
  vec3 mv;
  mv = pos + ins_pos;
  
  vs_id       = mesh_id;
  vs_uv       = uv;
  vs_normal   = normal;
  vs_position = mv;
  
  mv -= cam_pos;
  mv  = qrot(mv, qconj(cam_rot));
  gl_Position = proj(mv, cam_prj);
}
