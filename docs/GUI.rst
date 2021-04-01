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
