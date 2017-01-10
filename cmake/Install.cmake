# Required Vars:
# ${TARGET_NAME}

SET (INSTALL_BINDIR bin)
IF(WIN32)
    SET(INSTALL_LIBDIR bin)
    SET(INSTALL_ARCHIVEDIR lib)
ELSE(WIN32)
    SET(INSTALL_LIBDIR lib${LIB_POSTFIX})
    SET(INSTALL_ARCHIVEDIR lib${LIB_POSTFIX})
ENDIF(WIN32)

SET (PROJECT_NAME_LOWERCASE)
STRING (TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
INSTALL (
	TARGETS ${TARGET_NAME}
	EXPORT ${PROJECT_NAME_LOWERCASE}-config
	RUNTIME DESTINATION ${INSTALL_BINDIR}
	BUNDLE  DESTINATION ${INSTALL_BINDIR}
	LIBRARY DESTINATION ${INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${INSTALL_ARCHIVEDIR}
)
