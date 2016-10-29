

#ifndef __CMS_MDM_H__
#define __CMS_MDM_H__

/*!\file cms_mdm.h
 * \brief Header file containing MDM functions that external
 *  management entities are allowed to call.
 *
 */

#include "cms.h"
#include "cms_eid.h"
#include "mdm_params.h"

/*!\enum MdmParamTypes
 * \brief Possible types for parameters in the MDM.
 *
 * The first 6 are based on TR69.  Broadcom does not define any
 * other types for now.
 */
typedef enum
{
   MPT_STRING = 0,    /**< string. */
   MPT_INTEGER = 1,   /**< Signed 32 bit integer. */
   MPT_UNSIGNED_INTEGER = 2, /**<Unsigned 32 bit integer. */
   MPT_BOOLEAN = 3,   /**< Either 1 (true) or 0 (false). */
   MPT_DATE_TIME = 4, /**< string, in UTC, in ISO 8601 date-format. */
   MPT_BASE64 = 5     /**< String representation of binary data. */
} MdmParamTypes;



/** Active notification for TR69;  Used in MdmNodeAttributes.notification. */
#define NDA_TR69_NO_NOTIFICATION      0

/** Passive notification for TR69;  Used in MdmNodeAttributes.notification. */
#define NDA_TR69_PASSIVE_NOTIFICATION 1

/** Active notification for TR69;  Used in MdmNodeAttributes.notification. */
#define NDA_TR69_ACTIVE_NOTIFICATION  2

#ifdef CT_MIDDLEWARE
#define NDA_ITMS_NOTIFICATION_READABLE      1
#define NDA_ITMS_REBOOT_NOTIFICATION_READABLE      2
#define NDA_ITMS_NOTIFICATION_OFF_WRITABLE      4
#define NDA_ITMS_NOTIFICATION_WRITABLE      5
#define NDA_ITMS_REBOOT_NOTIFICATION_WRITABLE      6
#define NDA_CTMDW_NOTIFICATION_READABLE      8
#define NDA_CTMDW_ITMS_NOTIFICATION_READABLE      9
#define NDA_CTMDW_REBOOT_ITMS_NOTIFICATION_READABLE      10
#define NDA_CTMDW_NOTIFICATION_WRITABLE      12
#define NDA_CTMDW_ITMS_NOTIFICATION_WRITABLE      13
#define NDA_CTMDW_REBOOT_ITMS_NOTIFICATIONW_WRITABLE      14
#endif


/** A structure for storing node attributes.
 *
 * The MdmNodeAttributes only has meaning when applied to a
 * parameter node.  However, to save memory, if all 
 * parameter nodes of an object instance have the same attributes, 
 * their attributes are stored in the next higher level structure,
 * which may be the MdmObjNode (indirect 0), the InstanceHeadNode (indirect 1),
 * or InstanceDescNode (indirect 2).
 *
 * The accessBitMask has 15 bits as defined by the NDA_ACCESS_xxx defines
 * in cms_eid.h.  The bitmap represents entities for which write access to
 * the specified parameter(s) is granted by the owner of the entire data model.
 *
 * The parameter notification attribute indicates
 * whether the CPE should include changed values
 * of the specified parameter in the inform message,
 * and whether the CPE must initiate a session to
 * the TR-069 ACS when the specified parameter(s)
 * change in value. The following values are defined:
 *
 * 0 = Notification off.  The CPE need not inform 
 *       the ACS of a change to the specified
 *       parameter(s).
 *
 * 1 = Passive notification. Whenever the specified
 *       parameter value changes, TR-069C must include
 *       the new value in the ParameterList in the
 *       Inform message that is sent the next time a
 *       session is established to the ACS.
 *
 * 2 = Active notification. Whenever the specified
 *       parameter value changes, TR-069C must
 *       initiate a session to the ACS, and include
 *       the new value in the ParameterList in the
 *       associated Inform message.
 */
typedef struct
{
   UINT16  accessBitMaskChange:1; /**< Used during set, indicates whether the accessBitMask should be set */
   UINT16  accessBitMask:15;      /**< Bit field containing access bit mask, see NDA_ACCESS_xxx. */
   UINT8   notificationChange:1;  /**< Used during set, indicates whether the notifiction field should be set */
   UINT8   notification:7;        /**< Type of notification requested. */
   UINT8   valueChanged:1;        /**< This param has active or passive notification, and its value has changed.
                                   *   This field is used by MDM internal storage.  Not used during set. */
   UINT8   reserved:7;            /**< Reserved bit fields. */
} MdmNodeAttributes;



/** Default notification value is no notification. */
#define DEFAULT_NOTIFICATION_VALUE   NDA_TR69_NO_NOTIFICATION

/** Default access list bitmask is write access granted to everybody. */
#define DEFAULT_ACCESS_BIT_MASK   (NDA_ACCESS_TR69C | NDA_ACCESS_SUBSCRIBER)

/** definition of emtpy MdmNodeAttributes, this can be used to set an
 * MdmNodeAttributes variable declared on the stack to the empty state.
 */
#define EMPTY_NODE_ATTRIBUTES {0,0,0,0,0,0}


/** Macro to zeroize/initialize a MdmNodeAttributes */
#define INIT_NODE_ATTRIBUTES(s)  (memset((void *)(s), 0, sizeof(MdmNodeAttributes)))

/** Test whether there is a paramName in MdmPathDescriptor */
#define IS_PARAM_NAME_PRESENT(p) ((p)->paramName[0] != 0)

/** Macro to zeroize/initialize the paramname field of MdmPathDescriptor */
#define INIT_MDM_PARAM_NAME(p) (memset((void *)(p), 0, MAX_MDM_PARAM_NAME_LENGTH+1))

#endif /* __CMS_MDM_H__ */


