#include <irrlicht.h>
#include "driverChoice.h"
#include "ui/Projet_Depixelisation.h"
#include "src/similarity_graph.h"
#include "src/voronoi_diagram.h"
#include "src/spline.h"
#include "src/optimize.h"
#include <stdio.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
    IrrlichtDevice* device;
    s32             counter;
    IGUIListBox* listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_NEW_WINDOW_BUTTON,
    GUI_ID_FILE_OPEN_BUTTON,
    GUI_ID_TRANSPARENCY_SCROLL_BAR,
    GUI_ID_VORONOI,
    GUI_ID_UNION,
    GUI_ID_SPLINES,
    GUI_ID_DEPIXELISATION
};

s32 id_save;

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver(SAppContext& context) : Context(context) { }


    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = Context.device->getGUIEnvironment();

            switch (event.GUIEvent.EventType)
            {
            case EGET_BUTTON_CLICKED:
                switch (id)
                {
                case GUI_ID_QUIT_BUTTON:
                    Context.device->closeDevice();
                    return true;

                case GUI_ID_FILE_OPEN_BUTTON:
                    Context.listbox->addItem(L"Choix Image");
                    env->addFileOpenDialog(L"Choisissez un pixel art", true, 0, -1, true);
                    id_save = id;
                    return true;

                case GUI_ID_VORONOI:
                    Context.listbox->addItem(L"Choix Image");
                    env->addFileOpenDialog(L"Choisissez un pixel art", true, 0, -1, true);
                    id_save = id;
                    return true;

                case GUI_ID_UNION:
                    Context.listbox->addItem(L"Choix Image");
                    env->addFileOpenDialog(L"Choisissez un pixel art", true, 0, -1, true);
                    id_save = id;
                    return true;


                case GUI_ID_SPLINES:
                    Context.listbox->addItem(L"Choix Image");
                    env->addFileOpenDialog(L"Choisissez un pixel art", true, 0, -1, true);
                    id_save = id;
                    return true;


                case GUI_ID_DEPIXELISATION:
                    Context.listbox->addItem(L"Choix Image");
                    env->addFileOpenDialog(L"Choisissez un pixel art", true, 0, -1, true);
                    id_save = id;
                    return true;

                default:
                    return false;
                }
                break;

            case EGET_FILE_SELECTED:
            {
                //Appelle l'algorithe de dépixelisation avec l'argument représentant le chemin de l'image à traiter
                IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                
                Context.listbox->addItem(dialog->getFileName());
                s32 id = event.GUIEvent.Caller->getID();
                switch (id_save)
                {
                case GUI_ID_FILE_OPEN_BUTTON:
                    Context.listbox->addItem(L"Done");
                    env->addFileOpenDialog(L"Choisissez une autre image", true, 0, -1, true);
                    OpenCv(dialog->getFileName());
                    break;

                case GUI_ID_VORONOI:
                    Voronoi(dialog->getFileName());
                    Context.listbox->addItem(L"Voronoi");
                    break;

                case GUI_ID_UNION:
                    Union(dialog->getFileName());
                    Context.listbox->addItem(L"Union");
                    break;

                case GUI_ID_SPLINES:
                    Splines(dialog->getFileName());
                    Context.listbox->addItem(L"Splines");
                    break;

                case GUI_ID_DEPIXELISATION:
                    Depix(dialog->getFileName());
                    Context.listbox->addItem(L"Depixelisation");
                    break;

                default:
                    std::cout << "fail" << std::endl;
                }
            }
            break;

            default:
                break;
            }
        }

        return false;
    }

private:
    SAppContext& Context;
};

int main()
{
    // ask user for driver
    video::E_DRIVER_TYPE driverType = driverChoiceConsole();
    if (driverType == video::EDT_COUNT)
        return 1;

    // create device and exit if creation failed

    IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(540, 540));

    if (device == 0)
        return 1; // could not create selected driver.
    device->setWindowCaption(L"Depixelisation par vectorisation");
    device->setResizable(true);

    video::IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("Font/fontcourier.bmp");
    if (font)
        skin->setFont(font);

    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

    // Création des boutons
    env->addButton(rect<s32>(10, 240 - 40, 200, 240 + 144 - 120), 0, GUI_ID_FILE_OPEN_BUTTON,
        L"Graph similitude", L"Similitude");
    env->addButton(rect<s32>(220, 240 - 40, 410, 240 + 144 - 120), 0, GUI_ID_VORONOI,
        L"Diagramme de Voronoi", L"Voronoi");
    env->addButton(rect<s32>(10, 320 - 40, 200, 320 + 144 - 120), 0, GUI_ID_UNION,
        L"Union cellules", L"Union");
    env->addButton(rect<s32>(220, 320 - 40, 410, 320 + 144 - 120), 0, GUI_ID_SPLINES,
        L"Courbes parametriques", L"Splines");
    env->addButton(rect<s32>(10, 400 - 40, 200, 400 + 144 - 120), 0, GUI_ID_DEPIXELISATION,
        L"Depixelisation", L"Depixelisation");
    env->addButton(rect<s32>(220, 400 - 40, 410, 400 + 144 - 120), 0, GUI_ID_QUIT_BUTTON,
        L"Quit", L"Exit Programm");


    env->addStaticText(L"Console:", rect<s32>(10, 110 - 100, 410, 130 - 100), true);
    IGUIListBox* listbox = env->addListBox(rect<s32>(10, 140 - 100, 410, 210 - 20));


    // Store the appropriate data in a context structure.
    SAppContext context;
    context.device = device;
    context.counter = 0;
    context.listbox = listbox;

    // Then create the event receiver, giving it that context structure.
    MyEventReceiver receiver(context);

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(&receiver);
    env->addImage(driver->getTexture("../../media/irrlichtlogo2.png"),
        position2d<int>(10, 10));

    while (device->run() && driver)
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, SColor(0,120, 80, 200));

            env->drawAll();

            driver->endScene();
        }

    device->drop();

    return 0;
}
