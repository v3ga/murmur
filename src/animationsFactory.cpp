//
//  animationsFactory.cpp
//  murmur
//
//  Created by Julien on 16/05/13.
//
//

#include "animationsFactory.h"
#include "animations.h"

#include "animationWaveShader.h"
#include "animationParticlesMega2.h"
#include "animationBox2D.h"
#include "animationTunnel.h"
#include "animationTunnelALB.h"
#include "animationOrbit.h"
#include "animationGravitatory.h"
#include "animationComposition.h"
#include "animationWaves.h"
#include "animationVideo.h"
#include "animationEmpty.h"
#include "animationRadarFF.h"
#include "animationLinesFF.h"
#include "animationGridsFF.h"
#include "animationAgentsFF.h"

Animation* AnimationsFactory::create(string name)
{
    Animation* pAnimation=0;

    if (name == "shaderWave")
        pAnimation = new AnimationShaderWave(name);
/*    if (name == "particlesMega")
        pAnimation = new AnimationParticlesMega(name);
*/
    else if (name == "particlesMega2")
        pAnimation = new AnimationParticlesMega2(name);
    else if (name == "box2DCircles")
        pAnimation = new AnimationBox2D_circles(name);
    else if (name == "tunnel")
        pAnimation = new AnimationTunnel(name);
    else if (name == "tunnelALB")
        pAnimation = new AnimationTunnelALB(name);
    else if (name == "orbit")
        pAnimation = new AnimationOrbit(name);
    else if (name == "gravitatory")
        pAnimation = new AnimationGravitatory(name);
    else if (name == "composition")
        pAnimation = new AnimationComposition(name);
    else if (name == "waves")
        pAnimation = new AnimationWaves(name);
    else if (name == "video")
        pAnimation = new AnimationVideo(name);
    else if (name == "radarFF")
        pAnimation = new AnimationRadarFF(name);
    else if (name == "linesFF")
        pAnimation = new AnimationLinesFF(name);
    else if (name == "gridsFF")
        pAnimation = new AnimationGridsFF(name);
    else if (name == "agentsFF")
        pAnimation = new AnimationAgentsFF(name);
    else if (name == "empty")
        pAnimation = new AnimationEmpty(name);
 
    return pAnimation;
}