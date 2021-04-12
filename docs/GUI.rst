GUI
===

Default GUI Layout
^^^^^^^^^^^^^^^^^^

\TODO: Place a picture here and explain each element.

Setting up Data Editors
^^^^^^^^^^^^^^^^^^^^^^^

In order to get the GUISystem to see your component's data, you have to register the component. The GUISystem will display "Unregistered Component" for every component that it detects but does not have a display function for.

To register your component, call the defineGui function in the initEvent for your component ::

    SYSTEM(MySystem)

    void drawGui(const guiMeta& meta) {
        MyType* instance = meta_cast<MyType>(meta);
        doThings(*instance);
    }

    init(const ProjInitEvent& event) {
        defineGui<MyType>(drawGui);
    }
    END_SYSTEM

The defineGui function takes a ``void(const guiMeta&)`` function and uses this to draw the GUI for your component.

This function should be called when the project loads in your component. TODO: Figure out how this will work. Keep it at SceneInit for now, bc that's the current highest level of abstraction.

GuiTree
^^^^^^^

GuiTree is a system for nested trees. It relies on a list of Nodes, which contain reference to the type specified, as well as children. This makes them trees. They are designed to be used like a typical file directory.

Right click options are accessed through the Clipboard class.

GUI Events
^^^^^^^^^^

  GUI Events follow a simple cascade pattern. Each GUI Event is an implementation of one of these events:
    .. list-table:: **GUI Events**
        :widths: 30, 100
        :header-rows: 0

        * - ChangeEvent<T>
          - Fires when any change occurs; use this when it doesn't matter what type of event has occurred.
        * - CreateEvent<T>
          - Fires when the type is created.
        * - DestroyEvent<T>
          - Fires when the type is destroyed.
        * - ActivateEvent<T>
          - Fires when the type is made active in the scene.
        * - DeactivateEvent<T>
          - Fires when the type is made inactive in the scene.

  GUI Events cascade, as follows::

    ChangeEvent<Field> -> ChangeEvent<Component> -> ChangeEvent<Entity> -> ChangeEvent<Scene> -> ChangeEvent<Project>;



GUI Call Tree
^^^^^^^^^^^^^

::

  init -> checkContext
       -> initGuiFrontend -> initInspector 
  
  <RenderGuiEvent> -> render -> primaryContainer.renderFunc -> stateSetter -> setState -> <GuiEvent>
                                                           |-> mainMenu -> saveProject -> ...
                                                           |           |-> loadProject -> ...
                                                           |           |-> setState -> <GuiEvent>
                                                           |-> stateRouter -> Inspector -> RenderGuiTree -> RenderListNode -> node callbacks
                                                                          |            |-> RenderTabs -> tab.render
                                                                          |-> editorFrame.
                                                                          |-> editorToolbar.

  <>

The Inspector Window
^^^^^^^^^^^^^^^^^^^^

  The Inspector window is located at the bottom third of the screen. It has four sections: Scenes, Entities, Components, and Data.
  The Scenes section lists all the scenes in the current project.
  The Entities section lists all of the entities (active and inactive) in the currently active scene.
  The Components section lists all of the components of the selected Entity.
  And finally, the Data section shows a UI for the selected component.

GUI Meta
^^^^^^^^

  The GUI Meta interacts with user components in order to register them for runtime reflection. This allows users to create component user interfaces in the Data section of the Inspector window.

  Users interact with the ``GuiMeta`` type when they call ``defineGui<T>()`` in their ``init()`` function. ::

    SYSTEM(MySystem)
      void init(const InitEvent& ev) {
        defineGui<MyType>([](const guiMeta& meta){
          using namespace ImGui;
          MyType* myObj = meta_cast<MyType>(meta);

          //use ImGui to build your UI here.

        });
      }
    END_SYSTEM

  Unfortunately, I did not see a way to escape this boilerplate; but at least it is minimal.
  
  The ``defineGui<T>()`` function registers the component type and stores its information in a ``GuiMeta`` vector.

  Additionally, although EnTT comes with ``entt::registry::emplace<T>()``, users should opt for ``sundile::emplace<T>()`` for entities they wish to see on the GUI.

