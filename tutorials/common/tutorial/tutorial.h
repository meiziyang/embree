// ======================================================================== //
// Copyright 2009-2016 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../default.h"
//#include "tutorial_device.h"
#include "camera.h"
#include "scene.h"

namespace embree
{
  class TutorialApplication
  {
  public:
    enum Features { FEATURE_STREAM = 1 };

    TutorialApplication (const std::string& tutorialName, const int features = 0);
    
  public:
    
    /* virtual interface for command line option processing */
    struct CommandLineOption : public RefCount 
    {
    public:
      CommandLineOption (const std::string& description) 
        : description(description) {}

      virtual void parse(Ref<ParseStream> cin, const FileName& path) = 0;
      
    public:
      std::string description;
    };
    
    /* helper class to provide parsing function via lambda function */
    template<typename F>
      struct CommandLineOptionClosure : public CommandLineOption
    {
      CommandLineOptionClosure (std::string description, const F& f) 
        : CommandLineOption(description), f(f) {}
      
      virtual void parse(Ref<ParseStream> cin, const FileName& path) {
        f(cin,path);
      }
      
    public:
      F f;
    };
    
    /* registers a command line option */
    template<typename F>
      void registerOption(const std::string& name, const F& f, const std::string& description) 
    {
      Ref<CommandLineOption> closure = new CommandLineOptionClosure<F>(description,f);
      commandLineOptionList.push_back(closure);
      commandLineOptionMap[name] = closure;
    }

    /* registers an alias for a command line option */
    void registerOptionAlias(const std::string& name, const std::string& alternativeName); 
    
    /* command line parsing */
    void parseCommandLine(int argc, char** argv);
    void parseCommandLine(Ref<ParseStream> cin, const FileName& path);

    /* prints help for all supported command line options */
    void printCommandLineHelp();

    /* command line options database */
  public:
    std::vector<         Ref<CommandLineOption> > commandLineOptionList;
    std::map<std::string,Ref<CommandLineOption> > commandLineOptionMap;
 
  public:
    /* starts tutorial */
    void run(int argc, char** argv);

    /* virtual main function, contains all tutorial logic */
    virtual int main(int argc, char** argv);

    /* callback called after command line parsing finished */
    virtual void postParseCommandLine() {}
   
    /* benchmark mode */
    void renderBenchmark();
    
    /* render to file mode */
    void renderToFile(const FileName& fileName);

    /* GLUT callback functions */
  public:
    virtual void keyboardFunc(unsigned char key, int x, int y);
    virtual void specialFunc(int key, int, int);
    virtual void clickFunc(int button, int state, int x, int y);
    virtual void motionFunc(int x, int y);
    virtual void displayFunc(void);
    virtual void reshapeFunc(int width, int height);
    virtual void idleFunc();
    
  public:
    std::string tutorialName;
    int features;
    
    /* embree configuration */
    std::string rtcore;
          
    /* render settings */
    Camera camera;
    Shader shader;
    
    /* framebuffer settings */
    size_t width;
    size_t height;

    /* image output settings */
    FileName outputImageFilename;

    /* benchmark mode settings */
    int skipBenchmarkFrames;
    int numBenchmarkFrames;
    int numBenchmarkRepetitions;
  
    /* window settings */
    bool interactive;
    bool fullscreen;
    size_t window_width;
    size_t window_height;
    int windowID;
    
    double time0;
    int debug_int0;
    int debug_int1;
    
    int mouseMode;
    int clickX, clickY;
    
    float speed;

  public:
    static TutorialApplication* instance;
  };

  class SceneLoadingTutorialApplication : public TutorialApplication
  {
  public:
    SceneLoadingTutorialApplication (const std::string& tutorialName);

    virtual int main(int argc, char** argv);

  public:
    TutorialScene obj_scene;
    Ref<SceneGraph::GroupNode> scene;
    bool convert_tris_to_quads;
    bool convert_bezier_to_lines;
    bool convert_hair_to_curves;
    FileName sceneFilename;
    int instancing_mode;
    std::string subdiv_mode;
  };
}
