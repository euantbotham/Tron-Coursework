#include "bouncingLogo.h"

bouncingLogo::bouncingLogo(int xStart, int yStart, BaseEngine* pEngine, int speedX, int speedY) 
   : DisplayableObject(xStart, yStart, pEngine, 100, 100, true), 
    m_speedX(speedX), m_speedY(speedY){
    image1 = ImageManager::loadImage("disk1.png", false);
    image2 = ImageManager::loadImage("disk2.png", false);
    currentImage = 0;
}

void bouncingLogo::virtDraw() {
    // Draw a simple rectangle as the "logo"
    if (currentImage == 0) {
        image1.renderImageWithMask(
            getEngine()->getForegroundSurface(),
            0, 0, // Source position in the image
            m_iCurrentScreenX, m_iCurrentScreenY, // Destination position
            image1.getWidth(), image1.getHeight() // Width and height
        );
    }
    else {
        image2.renderImageWithMask(
            getEngine()->getForegroundSurface(),
            0, 0, // Source position in the image
            m_iCurrentScreenX, m_iCurrentScreenY, // Destination position
            image2.getWidth(), image2.getHeight() // Width and height
        );
    }
}


void bouncingLogo::virtDoUpdate(int iCurrentTime) {
    // Update position
    m_iCurrentScreenX += m_speedX;
    m_iCurrentScreenY += m_speedY;

    // Check for collisions with the walls and bounce
    bool bounced = false;
    if (m_iCurrentScreenX <= 0 || m_iCurrentScreenX + m_iDrawWidth >= getEngine()->getWindowWidth()) {
        m_speedX = -m_speedX; // Reverse horizontal direction
        bounced = true;
    }
    if (m_iCurrentScreenY <= 0 || m_iCurrentScreenY + m_iDrawHeight >= getEngine()->getWindowHeight()) {
        m_speedY = -m_speedY; // Reverse vertical direction
        bounced = true;
    }


    // Change image if bounced
    if (bounced) {
        currentImage = 1 - currentImage; // Toggle between 0 and 1
    }

    // Redraw the object
    redrawDisplay();
}

