/** @file
 *
 *  A scene is a logical way of repsenting and managing what is currently happening
 *  in the game. For example, if we want to go to the inventory when 'I' is pressed,
 *  the @ref sh3_scenemanager changes (or rather, fade transitions) the current scene
 *  pointer to point to the inventory object (this are pre-generated and stored when a game is loaded
 *  so that the player's inventory items etc are retained). This means that the previous scene,
 *  i.e, the actual game, are NOT updated, and the state of everything remains as it is. As sound is global,
 *  (I assume at least as of the 9th of Jan), it will still remain playing, just as in the original SILENT HILL.
 *  Some scenes, however (such as the options menu), can be deleted, as they don't need to retain any data as
 *  it's all dumped to the configuration file anyway,
 *
 *  Each scene supplies it's own event handler so that different key presses can be mapped to different actions.
 *  For example, pushing enter when the player is acting on Heather must perform a different action to when the
 *  player pushes enter in the inventory.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 9-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

namespace sh3_state
{
    class sh3_scene
    {
    public:

        sh3_scene(){};

        /**
         *  Render function for this scene. All calls to OpenGL are made here.
         *
         *  @param dt The time it took for the last frame to finish.
         */
        virtual void Draw(float dt) = 0;

        /**
         *  Update function for this scene. All logic calls are made here.
         *
         *  @param dt The time it took for the last frame to finish.
         */
        virtual void Update(float dt) = 0;

    private:
    protected:
    };
}



#endif // SCENE_H_INCLUDED
