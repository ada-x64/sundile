Events
======

    Events form the heart of system interaction.
    They are handled by a special system called the
    EventSystem. The EventSystem sends events between
    different systems. Every system can send and recieve
    event messages of any type.

    sundile's event system is an abstraction and extension
    of EnTT's event system.
    `Read more about it here. <https://entt.docsforge.com/v3.6.0/events-signals-and-everything-in-between/>`_

Built In Events
^^^^^^^^^^^^^^^

    There are built-in events in the main game loop so 
    users can time their events correctly.
    They are listed in this table in the order they are
    triggered:

    **TODO: Complete this**

    .. list-table:: **Event Order - Initialization**
        :widths: 30, 100
        :header-rows: 1

        * - Event
          - Description
        * - WindowInitEvent         
          - Triggers on window initialization.
        * - GuiInitEvent            
          - Triggers on GUI initialization.
        * - initEvent               
          - Calls all user init() functions.
        * - SceneInitEvent          
          - Triggers on scene initialization. This typically sinks all of the events for user components.

    .. list-table:: **Event Order - Main Loop**
        :widths: 30, 100
        :header-rows: 1

        * - Event
          - Description
        * - preStepEvent 
          - Preliminary actions and setup before main step event.
        * - WindowEvent             
          - Any window events are handled with this tag.
        * - WindowInputEvent        
          - Input is passed from the window to the game.
        * - TypedWindowEvent        
          - Hopefully to be depricated.
        * - stepEvent               
          - Main step event. This is where almost everything happens.
        * - SceneStepEvent
          - This is where the scene updates.
        * - SceneInputEvent         
          - Where the scene interprets input.
        * - RenderEvent             
          - This is where the game rendering is done.
        * - RenderGuiEvent          
          - This is where the editor's GUI rendering is done.
        * - postStepEvent           
          - Mostly cleanup from the main step event.

    .. list-table:: **Event Order - Termination**
        :widths: 30, 100
        :header-rows: 1

        * - Event
          - Description
        * - WindowTerminateEvent    
          - Closes the window, typically right before termination.
        * - terminateEvent          
          - This is called when the program is set to exit.

Creating Events
^^^^^^^^^^^^^^^

    Events take the form of simple structs with any content
    you want them to have. However, they must inherit from 
    the `Event` struct. ::

        struct myEvent : Event {
            //...
        }

Setting up Events 
^^^^^^^^^^^^^^^^^

    Event dispersal relies on the ``init()`` function. ::

        COMPONENT(myType)
            int x,y,z;
        END_COMPONENT

        struct MyEvent : Event {
            myType instance;
        }

        SYSTEM(mySystem)

            void catchMyEvent(const MyEvent& event) {
                //...
            }

            void init(const SceneInitEvent& event) {
                event.evw->dispatcher.sink<MyEvent>.connect<catchMyEvent>();
            }
        END_SYSTEM

        SYSTEM(myOtherSystem)

        void callMyEvent(evw) {
            myEvent event({1,2,3});
            evw.dispatcher.trigger<MyEvent>(event);
        }
        END_SYSTEM
    
    This is the simple event wrapper that comes with EnTT.
    Please refer to the EnTT documentation on proper usage.
