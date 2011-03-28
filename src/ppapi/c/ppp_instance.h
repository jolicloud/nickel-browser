/* Copyright (c) 2010 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef PPAPI_C_PPP_INSTANCE_H_
#define PPAPI_C_PPP_INSTANCE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"

struct PP_InputEvent;
struct PP_Var;

#define PPP_INSTANCE_INTERFACE "PPP_Instance;0.4"

/**
 * @file
 * This file defines the PPP_Instance structure - a series of points to methods
 * that you must implement in your model.
 *
 */

/** @addtogroup Interfaces
 * @{
 */

/**
 * The PPP_Instance interface contains pointers to a series of functions that
 * you must implement in your module. These functions can be trivial (simply
 * return the default return value) unless you want your module
 * to handle events such as change of focus or input events (keyboard/mouse)
 * events.
 */

struct PPP_Instance {
  /**
   * This value represents a pointer to a function that is called when a new
   * module is instantiated on the web page. The identifier of the new
   * instance will be passed in as the first argument (this value is
   * generated by the browser and is an opaque handle). This is called for each
   * instantiation of the NaCl module, which is each time the <embed> tag for
   * this module is encountered.
   *
   * It's possible for more than one module instance to be created
   * (i.e. you may get more than one OnCreate without an OnDestroy
   * in between).
   *
   * If this function reports a failure (by returning @a PP_FALSE), the NaCl
   * module will be deleted and DidDestroy will be called.
   *
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   * @param[in] argc The number of arguments contained in @a argn and @a argv.
   * @param[in] argn An array of argument names.  These argument names are
   * supplied in the <embed> tag, for example:
   * <embed id="nacl_module" dimensions="2"> will produce two argument
   * names: "id" and "dimensions."
   * @param[in] argv An array of argument values.  These are the values of the
   * arguments listed in the <embed> tag, for example
   * <embed id="nacl_module" dimensions="2"> will produce two argument
   * values: "nacl_module" and "2."  The indices of these values match the
   * indices of the corresponding names in @a argn.
   * @return @a PP_TRUE on success.
   */
  PP_Bool (*DidCreate)(PP_Instance instance,
                       uint32_t argc,
                       const char* argn[],
                       const char* argv[]);

  /**
   * This value represents a pointer to a function that is called when the
   * module instance is destroyed. This function will always be called,
   * even if DidCreate returned failure. The function should deallocate any data
   * associated with the instance.
   *
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   */
  void (*DidDestroy)(PP_Instance instance);

  /**
   * This value represents a pointer to a function that is called when the
   * position, the size, or the clip rectangle of the element in the
   * browser that corresponds to this NaCl module has changed.
   *
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   * @param[in] position The location on the page of this NaCl module. This is
   * relative to the top left corner of the viewport, which changes as the
   * page is scrolled.
   * @param[in] clip The visible region of the NaCl module. This is relative to
   * the top left of the plugin's coordinate system (not the page).  If the
   * plugin is invisible, @a clip will be (0, 0, 0, 0).
   */
  void (*DidChangeView)(PP_Instance instance,
                        const struct PP_Rect* position,
                        const struct PP_Rect* clip);

  /**
   * This value represents a pointer to a function to handle when your module
   * has gained or lost focus. Having focus means that keyboard events will be
   * sent to the module. A module's default condition is that it will
   * not have focus.
   *
   * Note: clicks on modules will give focus only if you handle the
   * click event. Return @a true from HandleInputEvent to signal that the click
   * event was handled. Otherwise the browser will bubble the event and give
   * focus to the element on the page that actually did end up consuming it.
   * If you're not getting focus, check to make sure you're returning true from
   * the mouse click in HandleInputEvent.
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   * @param[in] has_focus Indicates whether this NaCl module gained or lost
   * event focus.
   */
  void (*DidChangeFocus)(PP_Instance instance, PP_Bool has_focus);

  /**
   * This value represents a pointer to a function to handle input events.
   * Returns true if the event was handled or false if it was not.
   *
   * If the event was handled, it will not be forwarded to the web page or
   * browser. If it was not handled, it will bubble according to the normal
   * rules. So it is important that a module respond accurately with whether
   * event propogation should continue.
   *
   * Event propogation also controls focus. If you handle an event like a mouse
   * event, typically your module will be given focus. Returning false means
   * that the click will be given to a lower part of the page and your module
   * will not receive focus. This allows a module to be partially transparent,
   * where clicks on the transparent areas will behave like clicks to the
   * underlying page.
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   * @param[in] event The event.
   * @return PP_TRUE if @a event was handled, PP_FALSE otherwise.
   */
  PP_Bool (*HandleInputEvent)(PP_Instance instance,
                              const struct PP_InputEvent* event);

  /**
   * This value represents a pointer to a function that is called after
   * initialize for a full-frame plugin that was instantiated based on the MIME
   * type of a DOMWindow navigation. This only applies to modules that are
   * registered to handle certain MIME types (not current Native Client
   * modules).
   *
   * The given url_loader corresponds to a PPB_URLLoader instance that is
   * already opened. Its response headers may be queried using
   * PPB_URLLoader::GetResponseInfo. The url loader is not addrefed on behalf
   * of the module, if you're going to keep a reference to it, you need to
   * addref it yourself.
   *
   * This method returns PP_FALSE if the module cannot handle the data. In
   * response to this method, the module should call ReadResponseBody to read
   * the incoming data.
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   * @param[in] url_loader A PP_Resource an open PPB_URLLoader instance.
   * @return PP_TRUE if the data was handled, PP_FALSE otherwise.
   */
  PP_Bool (*HandleDocumentLoad)(PP_Instance instance, PP_Resource url_loader);

  /**
   * This value represents a pointer to a function that returns a Var
   * representing the scriptable object for the given instance. Normally
   * this will be a PPP_Class object that exposes certain methods the page
   * may want to call.
   *
   * On Failure, the returned var should be a "void" var.
   *
   * The returned PP_Var should have a reference added for the caller, which
   * will be responsible for Release()ing that reference.
   *
   * @param[in] instance A PP_Instance indentifying one instance of a module.
   * @return A PP_Var containing scriptable object.
   */
  struct PP_Var (*GetInstanceObject)(PP_Instance instance);
};
/**
 * @}
 */

#endif  /* PPAPI_C_PPP_INSTANCE_H_ */
