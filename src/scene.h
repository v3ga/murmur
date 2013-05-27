//
//  scene.h
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#pragma once

class Scene
{
    public:
        virtual void    setup       ()=0;
        virtual void    update      (float dt)=0;
        virtual void    draw        ()=0;

        virtual void    keyPressed  (int key)=0;
        virtual void    mousePressed(int x, int y)=0;
        virtual void    mouseDragged(int x, int y)=0;
        virtual void    mouseReleased(int x, int y)=0;
};



