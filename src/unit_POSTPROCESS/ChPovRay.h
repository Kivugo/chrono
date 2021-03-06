//
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2011-2012 Alessandro Tasora
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file at the top level of the distribution
// and at http://projectchrono.org/license-chrono.txt.
//

#ifndef CHPOVRAY_H
#define CHPOVRAY_H

//////////////////////////////////////////////////
//
//   ChPovRay.h
//
//
//   HEADER file for CHRONO,
//	 Multibody dynamics engine
//
// ------------------------------------------------
//             www.deltaknowledge.com
// ------------------------------------------------
///////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <sstream>
#include "physics/ChSystem.h"
#include "ChPostProcessBase.h"
#include "core/ChHashTable.h"
#include "core/ChHashFunction.h"
#include "assets/ChVisualization.h"

namespace chrono {

/// Namespace with classes for the postprocess unit.
namespace postprocess {

/// Class for post processing implementation that generates
/// scripts for POVray. The script can be used in POVray to
/// render photo-realistic animations.
/// It will convert only objects that contain supported
/// visualization assets AND that are flagged by attaching
/// a ChPovRayAsset to them (the ChPovRay::Add() function is
/// a shortcut for adding such ChPovRayAsset).

class ChApiPostProcess ChPovRay : public ChPostProcessBase {
  public:
    ChPovRay(ChSystem* system);
    virtual ~ChPovRay() {}

    enum eChContactSymbol {  // used for displaying contacts
        SYMBOL_VECTOR_SCALELENGTH = 0,
        SYMBOL_VECTOR_SCALERADIUS,
        SYMBOL_VECTOR_NOSCALE,
        SYMBOL_SPHERE_SCALERADIUS,
        SYMBOL_SPHERE_NOSCALE
    };

    /// Add a ChPhysicsItem object to the list of objects
    /// to render (if it has some associated ChAsset object).
    /// Note that this simply 'flags' the object as renderable
    /// but attaching a ChPovRayAsset to it.
    virtual void Add(ChSharedPtr<ChPhysicsItem> mitem);

    /// Remove a ChPhysicsItem object from the list of objects to render
    virtual void Remove(ChSharedPtr<ChPhysicsItem> mitem);

    /// Add all ChPhysicsItem objects in the system to
    /// the list of objects to render. Call this at the
    /// beginning of the simulation, for instance.
    virtual void AddAll();

    /// Remove all ChPhysicsItem objects that were previously added.
    virtual void RemoveAll();

    /// Tell if a ChPhysicsItem has been already added.
    virtual bool IsAdded(ChSharedPtr<ChPhysicsItem> mitem);

    /// Set the filename of the template for the script generation. If not set,
    /// it defaults to "_template_POV.pov" in the default Chrono data directory.
    virtual void SetTemplateFile(const std::string& filename) { template_filename = filename; }
    /// Set the filename of the .bmp files generated by POV.
    /// It should NOT contain the .bmp suffix because POV will append
    /// the frame number (es. pic0001.bmp, pic0002.bmp, ...).
    /// It can contain a directory (es. "myframes/pic"), but the
    /// directory must already exist.
    /// If not set, it defaults to "pic".
    virtual void SetPictureFilebase(const std::string& filename) { pic_filename = filename; }

    /// Set the filename of the output script (to be used in POV) generated
    /// by the function ExportScript(), es: "my_render.pov"
    /// It can contain a directory (es. "folder/my_render.pov"), but the
    /// directory must already exist.
    /// If not set, it defaults to "render_frames.pov".
    virtual void SetOutputScriptFile(const std::string& filename) { out_script_filename = filename; }

    /// Set the filename of the output data generated
    /// by the function ExportData(), es: "state"; when the user will
    /// execute POV and run the .ini file generated by ExportScript(),
    /// the .ini script will call the .pov script that will load these data files.
    /// It should NOT contain the .bmp suffix because ExportData()
    /// will append the frame number (es. state0001.dat, state0002.dat, ...).
    /// It can contain a directory (es. "output_folder/state"), but the
    /// directory must already exist.
    /// If not set, it defaults to "state".
    virtual void SetOutputDataFilebase(const std::string& filename) { out_data_filename = filename; }

    /// Set the picture width and height - will write this in the output .ini file.
    virtual void SetPictureSize(unsigned int width, unsigned int height) {
        picture_width = width;
        picture_height = height;
    };

    /// Set antialiasing - will write this in the output .ini file.
    virtual void SetAntialiasing(bool active, unsigned int depth, double treshold) {
        antialias = active;
        antialias_depth = depth;
        antialias_treshold = treshold;
    };

    /// Set the default camera position and aim point - will write this in the output .pov file.
    virtual void SetCamera(ChVector<> location, ChVector<> aim, double angle, bool ortho = false);

    /// Set the default light position and color - will write this in the output .pov file.
    virtual void SetLight(ChVector<> location, ChColor color, bool cast_shadow);

    /// Set the background color - will write this in the output .pov file.
    virtual void SetBackground(ChColor color) { background = color; }

    /// Set the ambiant light - will write this in the output .pov file.
    virtual void SetAmbientLight(ChColor color) { ambient_light = color; }

    /// Turn on/off the display of the COG (center of mass) of rigid bodies.
    /// If setting true, you can also set the size of the symbol, in meters.
    virtual void SetShowCOGs(bool show, double msize = 0.04);

    /// Turn on/off the display of the reference coordsystems of rigid bodies.
    /// If setting true, you can also set the size of the symbol, in meters.
    virtual void SetShowFrames(bool show, double msize = 0.05);

    /// Turn on/off the display of the reference coordsystems for ChLinkMate constraints.
    /// If setting true, you can also set the size of the symbol, in meters.
    virtual void SetShowLinks(bool show, double msize = 0.04);

    /// Turn on/off the display of contacts, using spheres or arrows (see eChContactSymbol modes).
    /// The size of the arrow or of the sphere dependson force strength multiplied by 'scale'.
    /// Use 'max_size' to limit size of arrows if too long, or spheres if too large (they will be signaled by white
    /// color)
    /// Use 'width' for the radius of the arrow. If in 'SYMBOL_VECTOR_SCALERADIUS' mode, the length of the vector is
    /// always max_size.
    virtual void SetShowContacts(bool show,
                                 eChContactSymbol mode,
                                 double scale,
                                 double width,
                                 double max_size,
                                 bool do_colormap,
                                 double colormap_start,
                                 double colormap_end);

    /// Set a string (a text block) of custom POV commands that you can optionally
    /// append to the POV script file, for example adding other POV lights, materials, etc.
    /// What you put in this string will be put at the end of the generated POV script, just
    /// before loading the data.
    virtual void SetCustomPOVcommandsScript(const std::string& mtext) { this->custom_script = mtext; }
    virtual const std::string& GetCustomPOVcommandsScript() { return this->custom_script; }

    /// Set a string (a text block) of custom POV commands that you can optionally
    /// append to the POV script files that are load at each timestep,
    /// es. state0001.pov, state0002.pov, for example adding other POV lights, materials, etc.
    virtual void SetCustomPOVcommandsData(const std::string& mtext) { this->custom_data = mtext; }
    virtual const std::string& GetCustomPOVcommandsData() { return this->custom_data; }

    /// When ExportData() is called, it saves .dat files in incremental
    /// way, starting from zero: data0000.dat, data0001.dat etc., but you can
    /// override the formatted number by calling SetFramenumber(), before.
    virtual void SetFramenumber(unsigned int mn) { this->framenumber = mn; }

    /// This function is used to export the script that will
    /// be used by POV to process all the exported data and
    /// to render the complete animation.
    /// It contains the definition of geometric shapes, lights
    /// and so on, and a POV function that moves meshes in the
    /// position specified by data files saved at each step.
    virtual void ExportScript() { this->ExportScript(this->out_script_filename); }
    /// As ExportScript(), but overrides the filename.
    virtual void ExportScript(const std::string& filename);

    /// This function is used at each timestep to export data
    /// formatted in a way that it can be load with the POV
    /// scripts generated by ExportScript().
    /// The generated filename must be set at the beginning of
    /// the animation via SetOutputDataFilebase(), and then a
    /// number is automatically appended and incremented at each
    /// ExportData(), ex.
    ///  state0001.dat, state0002.dat,
    /// The user should call this function in the while() loop
    /// of the simulation, once per frame.
    virtual void ExportData() {
        char fullpath[200];
        sprintf(fullpath, "%s%05d", this->out_data_filename.c_str(), this->framenumber);
        this->ExportData(std::string(fullpath));
    }
    /// As ExportScript(), but overrides the automatically computed filename.
    virtual void ExportData(const std::string& filename);

  protected:
    virtual void SetupLists();
    virtual void ExportAssets();
    void _recurseExportAssets(std::vector<ChSharedPtr<ChAsset> >& assetlist, ChStreamOutAsciiFile& assets_file);

    void _recurseExportObjData(std::vector<ChSharedPtr<ChAsset> >& assetlist,
                               ChFrame<> parentframe,
                               ChStreamOutAsciiFile& mfilepov);

    std::vector<ChSharedPtr<ChPhysicsItem> > mdata;
    ChHashTable<size_t, ChSharedPtr<ChAsset> > pov_assets;

    std::string template_filename;
    std::string pic_filename;

    std::string out_script_filename;
    std::string out_data_filename;

    unsigned int framenumber;

    ChVector<> camera_location;
    ChVector<> camera_aim;
    ChVector<> camera_up;
    double camera_angle;
    bool camera_orthographic;
    bool camera_found_in_assets;

    ChVector<> def_light_location;
    ChColor def_light_color;
    bool def_light_cast_shadows;

    bool COGs_show;
    double COGs_size;
    bool frames_show;
    double frames_size;
    bool links_show;
    double links_size;
    bool contacts_show;
    double contacts_maxsize;
    double contacts_scale;
    eChContactSymbol contacts_scale_mode;
    double contacts_width;
    double contacts_colormap_startscale;
    double contacts_colormap_endscale;
    bool contacts_do_colormap;
    ChColor background;
    ChColor ambient_light;

    bool antialias;
    int antialias_depth;
    double antialias_treshold;

    unsigned int picture_width;
    unsigned int picture_height;

    std::string custom_script;
    std::string custom_data;
};

}  // end namespace
}  // end namespace

#endif
