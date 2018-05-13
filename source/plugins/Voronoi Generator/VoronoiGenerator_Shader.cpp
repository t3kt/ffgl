//
//  VoronoiGenerator_Shader.cpp
//  Gradients
//
//  Created by tekt on 5/11/18.
//

#include "../../lib/ffgl/utilities/utilities.h"
#include "VoronoiGenerator.h"


std::string voronoiGenVertexShader = STRINGIFY
(
void main() {
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_FrontColor = gl_Color;
}
);

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


// I've not seen anybody out there computing correct cell interior distances for Voronoi
// patterns yet. That's why they cannot shade the cell interior correctly, and why you've
// never seen cell boundaries rendered correctly.

// However, here's how you do mathematically correct distances (note the equidistant and non
// degenerated grey isolines inside the cells) and hence edges (in yellow):

// http://www.iquilezles.org/www/articles/voronoilines/voronoilines.htm
// https://www.shadertoy.com/view/ldl3W8

std::string voronoiGenFragmentShader = STRINGIFY(


#define ANIMATE

 uniform float iGlobalTime;
 uniform vec2 scale;
 uniform float rotation;
 uniform vec2 translate;
 uniform vec2 borderOffset;
 uniform vec3 enabled;
 uniform vec3 borderColor;
 uniform vec2 resolution;

 vec2 hash2( vec2 p )
{
  // texture based white noise
  //return texture( resolution.xy, (p+0.5)/256.0 ).xy;

  // procedural white noise
  return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

 vec3 voronoi( in vec2 x )
{
  vec2 n = floor(x);
  vec2 f = fract(x);

  //----------------------------------
  // first pass: regular voronoi
  //----------------------------------
  vec2 mg;
  vec2 mr;

  float md = 8.0;
  for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
      vec2 g = vec2(float(i),float(j));
      vec2 o = hash2( n + g );
#ifdef ANIMATE
      o = 0.5 + 0.5*sin( iGlobalTime + 6.2831*o );
#endif
      vec2 r = g + o - f;
      float d = dot(r,r);

      if( d<md )
      {
        md = d;
        mr = r;
        mg = g;
      }
    }

  //----------------------------------
  // second pass: distance to borders
  //----------------------------------
  md = 8.0;
  for( int j=-2; j<=2; j++ )
    for( int i=-2; i<=2; i++ )
    {
      vec2 g = mg + vec2(float(i),float(j));
      vec2 o = hash2( n + g );
#ifdef ANIMATE
      o = 0.5 + 0.5*sin( iGlobalTime + 6.2831*o );
#endif
      vec2 r = g + o - f;

      if( dot(mr-r,mr-r)>0.00001 )
        md = min( md, dot( 0.5*(mr+r), normalize(r-mr) ) );
    }

  return vec3( md, mr );
}

 mat2 rotate2d(float _angle){
   return mat2(cos(_angle),-sin(_angle),
               sin(_angle),cos(_angle));
 }
 void main()
{
  vec2 res = resolution;
  vec2 p = gl_FragCoord.xy/res.xx;

  p -= vec2(0.5, 0.5);
  p *= rotate2d(rotation);
  p += vec2(0.5, 0.5);
  p *= scale;
  p += translate;
  vec3 c = voronoi( 8.0*p.xy );

  vec3 col = vec3(0, 0, 0);
  // isolines
  if (enabled[2] > 0.5) {
  col = c.x*(0.5 + 0.5*sin(64.0*c.x))*vec3(1.0);
  }
  // borders
  if (enabled[0] > 0.5) {
    col = mix( borderColor, col, smoothstep( borderOffset[0], borderOffset[1], c.x ) );
  }

  //    col = texture(sTD2DInputs[0], c.xy).rgb;
  if (enabled[1] > 0.5) {
    col.rg += c.xy;
  }

  // feature points
  //float dd = length( c.yz );
  //col = mix( vec3(1.0,0.6,0.1), col, smoothstep( 0.0, 0.12, dd) );
  //col += vec3(1.0,0.6,0.1)*(1.0-smoothstep( 0.0, 0.04, dd));

  gl_FragColor = vec4(col,1.0);
}

);
