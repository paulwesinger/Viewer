#ifndef MENUIDS_H
#define MENUIDS_H

namespace MENUIDS {
    enum{
        MENU_Main = 0,
        MENU_File,
        MENU_Edit,

        MENU_Primitives,
        MENU_Drawing,
    };
}

namespace FILEMENU_IDS {
    enum {
        ITEM_New = 100,
        ITEM_Edit,
        ITEM_Exit
    };
}

namespace EDITMENU_IDS {
    enum {
        ITEM_Copy = 200,
        ITEM_Paste,
    };
}

namespace PRIMITIVEOBJECTS_IDS {
    enum {
        ITEM_Cube = 300,
        ITEM_Sphere,
        ITEM_Pramid
    };
}


#endif // MENUIDS_H
