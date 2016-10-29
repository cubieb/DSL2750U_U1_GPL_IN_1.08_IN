<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
   <xsl:import href="doxy2wg.cfg"/>
   <xsl:import href="wg_dtd.cfg"/>
   <xsl:template match="/">
      <xsl:element name="Document">
         <!-- complete reference -->
         <xsl:element name="Chapter">
            <xsl:attribute name="NewPage">Yes</xsl:attribute>
            <xsl:element name="Title">Data Types and Structure Reference</xsl:element>
            <xsl:element name="Paragraph">This chapter contains the reference of data types and structures of all modules.</xsl:element>
            <!-- typedef struct { int a; int b; } foo; -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;struct&apos; and string-length(normalize-space(briefdescription/para))]">
               <xsl:call-template name="generate_ref_chapter_compounddef">
                  <xsl:with-param name="TYPE">struct</xsl:with-param>
                  <xsl:with-param name="NAME">Structure</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;struct&apos; and string-length(normalize-space(briefdescription/para))]"/>
               </xsl:call-template>
            </xsl:if>
            <!-- typedef union { int a; int b; } foo; -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;union&apos; and string-length(normalize-space(briefdescription/para))]">
               <xsl:call-template name="generate_ref_chapter_compounddef">
                  <xsl:with-param name="TYPE">union</xsl:with-param>
                  <xsl:with-param name="NAME">Union</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;union&apos; and string-length(normalize-space(briefdescription/para))]"/>
               </xsl:call-template>
            </xsl:if>
            <!-- typedef enum { yellow, blue } colors; -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;enum&apos; and string-length(normalize-space(briefdescription/para))]">
               <xsl:call-template name="generate_ref_chapter_compounddef">
                  <xsl:with-param name="TYPE">enum</xsl:with-param>
                  <xsl:with-param name="NAME">Enumerator</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;enum&apos; and string-length(normalize-space(briefdescription/para))]"/>
               </xsl:call-template>
            </xsl:if>
            <!--
				&lt;xsl:if test=&quot;doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;enum&apos;]&quot;&gt;
					&lt;xsl:call-template name=&quot;generate_ref_chapter_compounddef&quot;&gt;
						&lt;xsl:with-param name=&quot;TYPE&quot;&gt;enum&lt;/xsl:with-param&gt;
						&lt;xsl:with-param name=&quot;NAME&quot;&gt;Enumerator&lt;/xsl:with-param&gt;
						&lt;xsl:with-param name=&quot;ELEMENTS&quot; select=&quot;doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;enum&apos;]&quot;/&gt;
					&lt;/xsl:call-template&gt;
				&lt;/xsl:if&gt;
-->
            <!-- typedef struct -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;typedef&apos; and string-length(normalize-space(briefdescription/para))]">
               <xsl:call-template name="generate_ref_chapter_memberdef">
                  <xsl:with-param name="TYPE">typedef</xsl:with-param>
                  <xsl:with-param name="NAME">Type Definition</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;typedef&apos;        and string-length(normalize-space(briefdescription/para))]"/>
               </xsl:call-template>
            </xsl:if>
            <!-- #define FOO 0x42 -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;define&apos; and string-length(normalize-space(briefdescription/para))      and not(starts-with(name,&apos;FIO&apos;) or starts-with(name,&apos;DSL_FIO&apos;) or starts-with(name,&apos;IFXPHONE_&apos;) or starts-with(name,&apos;PHONE_&apos;))]">
               <xsl:call-template name="generate_ref_chapter_memberdef">
                  <xsl:with-param name="TYPE">define</xsl:with-param>
                  <xsl:with-param name="NAME">Constant</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;define&apos;        and string-length(normalize-space(briefdescription/para))       and not(starts-with(name,&apos;FIO&apos;) or starts-with(name,&apos;DSL_FIO&apos;) or starts-with(name,&apos;IFXPHONE_&apos;) or starts-with(name,&apos;PHONE_&apos;))]"/>
               </xsl:call-template>
            </xsl:if>
            <!-- enum { yellow, blue }; -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;enum&apos; and string-length(normalize-space(briefdescription/para))]">
               <xsl:call-template name="generate_ref_chapter_memberdef">
                  <xsl:with-param name="TYPE">enum</xsl:with-param>
                  <xsl:with-param name="NAME">Enumerator</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;enum&apos; and string-length(normalize-space(briefdescription/para))]"/>
               </xsl:call-template>
            </xsl:if>
            <!-- foo() -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;function&apos; and string-length(normalize-space(briefdescription/para))]">
               <xsl:call-template name="generate_ref_chapter_memberdef">
                  <xsl:with-param name="TYPE">function</xsl:with-param>
                  <xsl:with-param name="NAME">Function</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;function&apos; and string-length(normalize-space(briefdescription/para))]"/>
               </xsl:call-template>
            </xsl:if>
            <!-- #define FIO_DO_FOO _IO(0x42) -->
            <xsl:if test="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;define&apos; and string-length(normalize-space(briefdescription/para))      and (starts-with(name,&apos;FIO&apos;) or starts-with(name,&apos;DSL_FIO&apos;) or starts-with(name,&apos;IFXPHONE_&apos;) or starts-with(name,&apos;PHONE_&apos;))]">
               <xsl:call-template name="generate_ref_chapter_memberdef">
                  <xsl:with-param name="TYPE">ioctl</xsl:with-param>
                  <xsl:with-param name="NAME">IO-control</xsl:with-param>
                  <xsl:with-param name="ELEMENTS" select="doxygen/compounddef[@kind=&apos;file&apos;]/sectiondef/memberdef[@kind=&apos;define&apos;        and string-length(normalize-space(briefdescription/para))       and (starts-with(name,&apos;FIO&apos;) or starts-with(name,&apos;DSL_FIO&apos;) or starts-with(name,&apos;IFXPHONE_&apos;) or starts-with(name,&apos;PHONE_&apos;))]"/>
               </xsl:call-template>
            </xsl:if>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print io-control -->
   <xsl:template match="memberdef" mode="ioctl">
      <xsl:element name="SW_Component">
         <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;name&quot;/&gt;&lt;/xsl:attribute&gt;-->
         <xsl:attribute name="Role">IO_Control</xsl:attribute>
         <xsl:element name="Title">
            <xsl:attribute name="Id"><xsl:value-of select="@id"/></xsl:attribute>
            <xsl:value-of select="name"/>
         </xsl:element>
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Description</xsl:attribute>
            <xsl:element name="Title"/>
            <xsl:element name="Paragraph">
               <xsl:value-of select="briefdescription/para"/>
            </xsl:element>
         </xsl:element>
         <xsl:call-template name="generate_synopsis"/>
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Parameters</xsl:attribute>
            <xsl:element name="Title"/>
            <xsl:element name="SW_Table">
               <xsl:element name="DataTypeTable">
                  <xsl:call-template name="generate_dt_head"/>
                  <!-- fd -->
                  <xsl:element name="DT_Body">
                     <xsl:element name="DT_Row">
                        <xsl:element name="DataType">int</xsl:element>
                        <xsl:element name="ParamName">fd</xsl:element>
                        <xsl:element name="SW_Description">
                           <DescriptionText>Pointer to a file descriptor</DescriptionText>
                        </xsl:element>
                     </xsl:element>
                     <!-- I/O identifier -->
                     <xsl:element name="DT_Row">
                        <xsl:element name="DataType">int</xsl:element>
                        <xsl:element name="ParamName">
                           <xsl:value-of select="normalize-space(name)"/>
                        </xsl:element>
                        <xsl:element name="SW_Description">
                           <DescriptionText>I/O control identifier for this operation</DescriptionText>
                        </xsl:element>
                     </xsl:element>
                     <!-- I/O parameter -->
                     <xsl:element name="DT_Row">
                        <xsl:choose>
                           <xsl:when test="count(detaileddescription/para/parameterlist/parameteritem/parameternamelist/parametername)">
                              <xsl:element name="DataType">
                                 <!--- IR: find ref by name and generate crossref  with template \\compoundef[compoundname= -->
                                 <xsl:variable name="PARMTYPE" select="normalize-space (detaileddescription/para/parameterlist/parameteritem/parameternamelist/parametername)"/>
                                 <xsl:call-template name="generate_ref">
                                    <xsl:with-param name="VALUE" select="$PARMTYPE"/>
                                 </xsl:call-template>
                              </xsl:element>
                              <xsl:element name="ParamName">param</xsl:element>
                              <xsl:element name="SW_Description">
                                 <xsl:element name="DescriptionText">
                                    <xsl:value-of select="detaileddescription/para/parameterlist/parameteritem/parameterdescription/para"/>
                                 </xsl:element>
                                 <xsl:apply-templates select="detaileddescription/para/itemizedlist" mode="valuelist"/>
                              </xsl:element>
                           </xsl:when>
                           <xsl:otherwise>
                              <xsl:element name="DataType">int</xsl:element>
                              <xsl:element name="ParamName">param</xsl:element>
                              <xsl:element name="SW_Description">
                                 <xsl:apply-templates select="detaileddescription/para/itemizedlist" mode="generate"/>
                              </xsl:element>
                           </xsl:otherwise>
                        </xsl:choose>
                     </xsl:element>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
         </xsl:element>
         <xsl:if test="detaileddescription/para/simplesect[@kind=&apos;return&apos;]">
            <xsl:element name="SW_Block">
               <xsl:attribute name="Role">ReturnValues</xsl:attribute>
               <xsl:element name="Title"/>
               <xsl:element name="SW_Table">
                  <xsl:element name="ReturnValuesTable">
                     <xsl:call-template name="generate_rv_head"/>
                     <xsl:element name="RV_Body">
                        <xsl:element name="RV_Row">
                           <xsl:element name="DataType">int</xsl:element>
                           <xsl:element name="SW_Description">
                              <xsl:apply-templates select="detaileddescription/para/simplesect[@kind=&apos;return&apos;]" mode="generate"/>
                           </xsl:element>
                        </xsl:element>
                     </xsl:element>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
         </xsl:if>
         <xsl:call-template name="generate_code_remark"/>
         <xsl:call-template name="generate_code_example"/>
      </xsl:element>
   </xsl:template>
   <!-- print compound (typedef handling) -->
   <xsl:template name="generate_compounddef">
      <xsl:param name="ROLE"/>
      <xsl:param name="ELEMENTS"/>
      <xsl:for-each select="$ELEMENTS">
         <xsl:element name="SW_Component">
            <xsl:attribute name="Id"><xsl:value-of select="compoundname"/></xsl:attribute>
            <xsl:attribute name="Role"><xsl:value-of select="$ROLE"/></xsl:attribute>
            <xsl:element name="Title">
               <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
               <xsl:value-of select="compoundname"/>
            </xsl:element>
            <xsl:call-template name="generate_description"/>
            <xsl:call-template name="generate_synopsis"/>
            <xsl:call-template name="generate_dt_table"/>
            <xsl:call-template name="generate_code_remark"/>
            <xsl:call-template name="generate_code_example"/>
         </xsl:element>
      </xsl:for-each>
   </xsl:template>
   <!-- print enum -->
   <xsl:template match="memberdef" mode="enum">
      <xsl:element name="SW_Component">
         <xsl:attribute name="Id"><xsl:value-of select="name"/></xsl:attribute>
         <xsl:attribute name="Role">Enum</xsl:attribute>
         <xsl:element name="Title">
            <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
            <xsl:value-of select="name"/>
         </xsl:element>
         <xsl:call-template name="generate_description"/>
         <xsl:call-template name="generate_synopsis"/>
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Parameters</xsl:attribute>
            <xsl:element name="Title"/>
            <SW_Table>
               <xsl:element name="DefinitionTable">
                  <xsl:call-template name="generate_d_head"/>
                  <xsl:element name="D_Body">
                     <xsl:for-each select="enumvalue">
                        <xsl:element name="D_Row">
                           <xsl:element name="DefName">
                              <xsl:attribute name="Id"><xsl:value-of select="name"/></xsl:attribute>
                              <xsl:value-of select="name"/>
                           </xsl:element>
                           <xsl:element name="Value">
                              <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
                              <xsl:value-of select="initializer"/>
                           </xsl:element>
                           <xsl:element name="SW_Description">
                              <xsl:apply-templates select="briefdescription"/>
                              <xsl:apply-templates select="detaileddescription"/>
                           </xsl:element>
                        </xsl:element>
                     </xsl:for-each>
                  </xsl:element>
               </xsl:element>
            </SW_Table>
         </xsl:element>
         <xsl:call-template name="generate_code_remark"/>
         <xsl:call-template name="generate_code_example"/>
      </xsl:element>
   </xsl:template>
   <!--print define -->
   <xsl:template match="memberdef" mode="define">
      <xsl:element name="SW_Component">
         <xsl:attribute name="Id"><xsl:value-of select="name"/></xsl:attribute>
         <xsl:attribute name="Role">Define</xsl:attribute>
         <xsl:element name="Title">
            <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
            <xsl:value-of select="name"/>
         </xsl:element>
         <!-- no description, will be added within the DT_Table -->
         <xsl:call-template name="generate_synopsis"/>
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Parameters</xsl:attribute>
            <xsl:element name="Title"/>
            <xsl:element name="SW_Table">
               <xsl:element name="DataTypeTable">
                  <xsl:call-template name="generate_dt_head"/>
                  <xsl:element name="DT_Body">
                     <xsl:element name="DT_Row">
                        <xsl:element name="DataType">
                           <xsl:value-of select="normalize-space(name)"/>
                        </xsl:element>
                        <xsl:element name="ParamName">
                           <xsl:value-of select="normalize-space(initializer)"/>
                        </xsl:element>
                        <xsl:element name="SW_Description">
                           <xsl:apply-templates select="briefdescription"/>
                           <xsl:apply-templates select="detaileddescription"/>
                        </xsl:element>
                     </xsl:element>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
         </xsl:element>
         <xsl:call-template name="generate_code_remark"/>
         <xsl:call-template name="generate_code_example"/>
      </xsl:element>
   </xsl:template>
   <!--print define -->
   <xsl:template match="memberdef" mode="typedef">
      <xsl:element name="SW_Component">
         <xsl:attribute name="Id"><xsl:value-of select="name"/></xsl:attribute>
         <xsl:choose>
            <xsl:when test="contains(normalize-space(type),&apos;(*&apos;)">
               <xsl:attribute name="Role">FunctionPointer</xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
               <xsl:attribute name="Role">TypeDefinition</xsl:attribute>
            </xsl:otherwise>
         </xsl:choose>
         <xsl:element name="Title">
            <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
            <xsl:value-of select="name"/>
         </xsl:element>
         <!-- no description, will be added within the DT_Table -->
         <xsl:call-template name="generate_synopsis"/>
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Parameters</xsl:attribute>
            <xsl:element name="Title"/>
            <xsl:element name="SW_Table">
               <xsl:element name="DataTypeTable">
                  <xsl:call-template name="generate_dt_head"/>
                  <xsl:element name="DT_Body">
                     <xsl:choose>
                        <xsl:when test="contains(normalize-space(type),&apos;(*&apos;)">
                           <xsl:apply-templates select="detaileddescription/para/parameterlist/parametername" mode="decIname"/>
                        </xsl:when>
                        <xsl:otherwise>
                           <xsl:element name="DT_Row">
                              <xsl:element name="DataType">
                                 <xsl:choose>
                                    <xsl:when test="contains(definition,&apos; struct &apos;)">
                                       <xsl:text>struct </xsl:text>
                                    </xsl:when>
                                    <xsl:when test="contains(definition,&apos; union &apos;)">
                                       <xsl:text>union </xsl:text>
                                    </xsl:when>
                                    <xsl:when test="contains(definition,&apos; enum &apos;)">
                                       <xsl:text>enum </xsl:text>
                                    </xsl:when>
                                    <xsl:otherwise>
												</xsl:otherwise>
                                 </xsl:choose>
                                 <xsl:apply-templates select="type"/>
                              </xsl:element>
                              <xsl:element name="ParamName">
                                 <xsl:value-of select="normalize-space(name)"/>
                              </xsl:element>
                              <xsl:element name="SW_Description">
                                 <xsl:apply-templates select="briefdescription"/>
                                 <xsl:apply-templates select="detaileddescription"/>
                              </xsl:element>
                           </xsl:element>
                        </xsl:otherwise>
                     </xsl:choose>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
         </xsl:element>
         <xsl:call-template name="generate_return_table"/>
         <xsl:call-template name="generate_code_remark"/>
         <xsl:call-template name="generate_code_example"/>
      </xsl:element>
   </xsl:template>
   <!-- print function -->
   <xsl:template match="memberdef" mode="function">
      <xsl:element name="SW_Component">
         <xsl:attribute name="Id"><xsl:value-of select="name"/></xsl:attribute>
         <xsl:attribute name="Role">Function</xsl:attribute>
         <xsl:element name="Title">
            <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
            <xsl:value-of select="name"/>
         </xsl:element>
         <xsl:call-template name="generate_description"/>
         <xsl:call-template name="generate_synopsis"/>
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Parameters</xsl:attribute>
            <xsl:element name="Title"/>
            <xsl:element name="SW_Table">
               <xsl:element name="DataTypeTable">
                  <xsl:call-template name="generate_dt_head"/>
                  <xsl:element name="DT_Body">
                     <xsl:apply-templates select="param" mode="decIname"/>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
         </xsl:element>
         <xsl:call-template name="generate_return_table"/>
         <!--
			&lt;xsl:call-template name=&quot;generate_code_example&quot;/&gt;
-->
         <xsl:call-template name="generate_code_remark"/>
      </xsl:element>
   </xsl:template>
   <!-- print function parameter within DT_Table -->
   <xsl:template match="param" mode="decIname">
      <xsl:variable name="DECLNAME" select="string(declname)"/>
      <xsl:element name="DT_Row">
         <xsl:element name="DataType">
            <!--				&lt;xsl:value-of select=&quot;normalize-space(type)&quot;/&gt; -->
            <xsl:apply-templates select="type"/>
         </xsl:element>
         <xsl:element name="ParamName">
            <xsl:value-of select="normalize-space(declname)"/>
         </xsl:element>
         <xsl:element name="SW_Description">
            <xsl:for-each select="..//parameterlist[@kind=&apos;param&apos;]//parametername[.=$DECLNAME]/following-sibling::*[1]">
               <DescriptionText>
                  <xsl:value-of select="para"/>
               </DescriptionText>
               <xsl:apply-templates select="para/itemizedlist" mode="valuelist"/>
            </xsl:for-each>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <xsl:template name="Extract">
      <xsl:param name="ToExtract"/>
      <xsl:param name="CurrIndex"/>
      <xsl:param name="Text"/>
      <xsl:choose>
         <xsl:when test="$CurrIndex&lt;$ToExtract">
            <xsl:call-template name="Extract">
               <xsl:with-param name="ToExtract" select="$ToExtract"/>
               <xsl:with-param name="CurrIndex" select="$CurrIndex+1"/>
               <xsl:with-param name="Text" select="substring-after($Text,&apos; &apos;)"/>
            </xsl:call-template>
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="translate(substring-before($Text,&apos; &apos;),&apos;,&apos;,&apos;&apos;)"/>
            <xsl:choose>
               <xsl:when test="substring-before($Text,&apos; &apos;)=substring-before($Text,&apos; *&apos;)">*</xsl:when>
               <xsl:otherwise>
					</xsl:otherwise>
            </xsl:choose>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
   <!-- print function-pointer parameter within DT_Table -->
   <xsl:template match="parametername" mode="decIname">
      <xsl:element name="DT_Row">
         <xsl:element name="DataType">
            <xsl:call-template name="Extract">
               <xsl:with-param name="ToExtract" select="position()*2"/>
               <xsl:with-param name="CurrIndex" select="1"/>
               <xsl:with-param name="Text" select="substring-after(../../../../definition,&apos;)(&apos;)"/>
            </xsl:call-template>
         </xsl:element>
         <xsl:element name="ParamName">
            <xsl:value-of select="normalize-space(.)"/>
         </xsl:element>
         <xsl:element name="SW_Description">
            <xsl:for-each select="./following-sibling::*[1]">
               <DescriptionText>
                  <xsl:value-of select="para"/>
               </DescriptionText>
               <xsl:apply-templates select="para/itemizedlist" mode="valuelist"/>
            </xsl:for-each>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print reference chapter for compounddef (typedef handling) -->
   <xsl:template name="generate_ref_chapter_compounddef">
      <xsl:param name="TYPE"/>
      <xsl:param name="NAME"/>
      <xsl:param name="ELEMENTS"/>
      <xsl:element name="Chapter">
         <xsl:element name="Title">
            <xsl:value-of select="$NAME"/> Reference</xsl:element>
         <xsl:element name="Paragraph">This chapter contains the <xsl:value-of select="$NAME"/> reference.</xsl:element>
         <xsl:choose>
            <xsl:when test="$TYPE=&apos;struct&apos;">
               <xsl:call-template name="generate_compounddef">
                  <xsl:with-param name="ELEMENTS" select="$ELEMENTS"/>
                  <xsl:with-param name="ROLE" select="&apos;Structure&apos;"/>
               </xsl:call-template>
            </xsl:when>
            <xsl:when test="$TYPE=&apos;union&apos;">
               <xsl:call-template name="generate_compounddef">
                  <xsl:with-param name="ELEMENTS" select="$ELEMENTS"/>
                  <xsl:with-param name="ROLE" select="&apos;Union&apos;"/>
               </xsl:call-template>
            </xsl:when>
            <xsl:when test="$TYPE=&apos;enum&apos;">
               <xsl:call-template name="generate_compounddef">
                  <xsl:with-param name="ELEMENTS" select="$ELEMENTS"/>
                  <xsl:with-param name="ROLE" select="&apos;Enum&apos;"/>
               </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
				</xsl:otherwise>
         </xsl:choose>
      </xsl:element>
   </xsl:template>
   <!-- print memberdef reference table -->
   <xsl:template name="generate_ref_chapter_memberdef">
      <xsl:param name="TYPE"/>
      <xsl:param name="NAME"/>
      <xsl:param name="ELEMENTS"/>
      <xsl:element name="Chapter">
         <xsl:element name="Title">
            <xsl:value-of select="$NAME"/> Reference</xsl:element>
         <xsl:element name="Paragraph">This chapter contains the <xsl:value-of select="$NAME"/> reference.</xsl:element>
         <xsl:choose>
            <xsl:when test="$TYPE=&apos;enum&apos;">
               <xsl:apply-templates select="$ELEMENTS" mode="enum">
                  <xsl:sort select="name" data-type="text" order="ascending"/>
               </xsl:apply-templates>
            </xsl:when>
            <xsl:when test="$TYPE=&apos;define&apos;">
               <xsl:apply-templates select="$ELEMENTS" mode="define">
                  <xsl:sort select="name" data-type="text" order="ascending"/>
               </xsl:apply-templates>
            </xsl:when>
            <xsl:when test="$TYPE=&apos;ioctl&apos;">
               <xsl:apply-templates select="$ELEMENTS" mode="ioctl">
                  <xsl:sort select="name" data-type="text" order="ascending"/>
               </xsl:apply-templates>
            </xsl:when>
            <xsl:when test="$TYPE=&apos;typedef&apos;">
               <xsl:apply-templates select="$ELEMENTS" mode="typedef">
                  <xsl:sort select="name" data-type="text" order="ascending"/>
               </xsl:apply-templates>
            </xsl:when>
            <xsl:when test="$TYPE=&apos;function&apos;">
               <xsl:apply-templates select="$ELEMENTS" mode="function">
                  <xsl:sort select="name" data-type="text" order="ascending"/>
               </xsl:apply-templates>
            </xsl:when>
            <xsl:otherwise>
				</xsl:otherwise>
         </xsl:choose>
      </xsl:element>
   </xsl:template>
   <!-- reference table / link to sub-groups -->
   <xsl:template name="generate_inner_group">
      <xsl:element name="Table">
         <xsl:element name="S_Table">
            <xsl:attribute name="frame">topbot</xsl:attribute>
            <xsl:attribute name="cols">2</xsl:attribute>
            <xsl:attribute name="colsep">1</xsl:attribute>
            <xsl:attribute name="rowsep">1</xsl:attribute>
            <xsl:attribute name="cwidths">1.830in 5.050in</xsl:attribute>
            <xsl:element name="TableTitle">
               <xsl:element name="TCaption">Module Overview<xsl:element name="TableContinuation"/>
               </xsl:element>
            </xsl:element>
            <xsl:element name="S_Head">
               <xsl:element name="S_HRow">
                  <xsl:attribute name="rowsep">1</xsl:attribute>
                  <xsl:element name="S_HCell">
                     <xsl:attribute name="colname">1</xsl:attribute>
                     <xsl:element name="S_HCellBody">Name</xsl:element>
                  </xsl:element>
                  <xsl:element name="S_HCell">
                     <xsl:attribute name="colname">2</xsl:attribute>
                     <xsl:element name="S_HCellBody">Description</xsl:element>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
            <xsl:element name="S_Body">
               <xsl:for-each select="innergroup">
                  <xsl:element name="S_Row">
                     <xsl:attribute name="rowsep">1</xsl:attribute>
                     <xsl:element name="S_Cell">
                        <xsl:attribute name="colname">1</xsl:attribute>
                        <xsl:element name="S_CellBody">
                           <xsl:element name="CrossRef">
                              <xsl:attribute name="Idref"><xsl:value-of select="@refid"/></xsl:attribute>
                              <xsl:attribute name="type">Text</xsl:attribute>
                           </xsl:element>
                        </xsl:element>
                     </xsl:element>
                     <xsl:element name="S_Cell">
                        <xsl:attribute name="colname">2</xsl:attribute>
                        <xsl:element name="S_CellBody">
                           <xsl:value-of select="."/>
                        </xsl:element>
                     </xsl:element>
                  </xsl:element>
               </xsl:for-each>
            </xsl:element>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print header for D_Table -->
   <xsl:template name="generate_d_head">
      <xsl:element name="D_Head">
         <xsl:element name="D_HRow">
            <xsl:element name="HDefName">
               <xsl:element name="AutoText"/>
            </xsl:element>
            <xsl:element name="HValue">
               <xsl:element name="AutoText"/>
            </xsl:element>
            <xsl:element name="HSW_Description">
               <xsl:element name="AutoText"/>
            </xsl:element>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print header for DT_Table -->
   <xsl:template name="generate_dt_head">
      <xsl:element name="DT_Head">
         <xsl:element name="DT_HRow">
            <xsl:element name="HDataType">
               <xsl:element name="AutoText"/>
            </xsl:element>
            <xsl:element name="HParamName">
               <xsl:element name="AutoText"/>
            </xsl:element>
            <xsl:element name="HSW_Description">
               <xsl:element name="AutoText"/>
            </xsl:element>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print header for ReturnValuesTable -->
   <xsl:template name="generate_rv_head">
      <xsl:element name="RV_Head">
         <xsl:element name="RV_HRow">
            <xsl:element name="HDataType">
               <xsl:element name="AutoText"/>
            </xsl:element>
            <xsl:element name="HSW_Description">
               <xsl:element name="AutoText"/>
            </xsl:element>
         </xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print empty synopsis block, should be updated with IFX toolbox -->
   <xsl:template name="generate_synopsis">
      <xsl:element name="SW_Block">
         <xsl:attribute name="Role">Prototype</xsl:attribute>
         <xsl:element name="Title"/>
         <xsl:element name="Code">
			</xsl:element>
      </xsl:element>
   </xsl:template>
   <!-- print description block -->
   <xsl:template name="generate_description">
      <xsl:element name="SW_Block">
         <xsl:attribute name="Role">Description</xsl:attribute>
         <xsl:element name="Title"/>
         <xsl:element name="Paragraph">
            <xsl:value-of select="briefdescription/para"/>
         </xsl:element>
         <xsl:for-each select="detaileddescription/para">
            <xsl:variable name="DESCRIPTION_LONG">
               <xsl:apply-templates select="."/>
            </xsl:variable>
            <xsl:variable name="DESCRIPTION_NORM" select="normalize-space($DESCRIPTION_LONG)"/>
            <xsl:if test="string-length($DESCRIPTION_NORM)">
               <Paragraph>
                  <xsl:value-of select="$DESCRIPTION_NORM"/>
               </Paragraph>
            </xsl:if>
            <xsl:apply-templates select="simplesect[@kind=&apos;attention&apos;]" mode="attention"/>
            <xsl:apply-templates select="simplesect[@kind=&apos;note&apos;]" mode="note"/>
            <xsl:apply-templates select="itemizedlist" mode="generate"/>
            <xsl:apply-templates select="orderedlist" mode="generate"/>
            <xsl:apply-templates select="programlisting/codeline" mode="generate"/>
         </xsl:for-each>
      </xsl:element>
   </xsl:template>
   <!-- print list of arguments ( \arg ) -->
   <xsl:template match="itemizedlist" mode="generate">
      <List>
         <xsl:for-each select="listitem/para">
            <Item>
               <xsl:apply-templates select="."/>
            </Item>
         </xsl:for-each>
      </List>
   </xsl:template>
   <xsl:template match="orderedlist" mode="generate">
      <xsl:for-each select="listitem/para">
         <xsl:variable name="DESCRIPTION_LONG">
            <xsl:apply-templates select="."/>
         </xsl:variable>
         <xsl:variable name="DESCRIPTION_NORM" select="normalize-space($DESCRIPTION_LONG)"/>
         <Paragraph>
            <xsl:value-of select="$DESCRIPTION_NORM"/>
         </Paragraph>
         <xsl:apply-templates select="itemizedlist" mode="generate"/>
      </xsl:for-each>
   </xsl:template>
   <!-- print list of arguments ( \arg ) for bit definitions 
	If we find a value, a &quot;:&quot; and a &quot;,&quot; then we generate a constant tag. Also 0x is wrapped with Hex, 
	otherwise Dec is wrapped around the value
	-->
   <xsl:template match="itemizedlist" mode="valuelist">
      <xsl:choose>
         <xsl:when test="contains(current(), &apos;:&apos;) and contains(current(), &apos;,&apos;) and not(contains(substring-after(substring-before(current(), &apos;,&apos;), &apos;: &apos;), &apos; &apos;))">
            <xsl:for-each select="listitem/para">
               <Constant>
                  <ConstantValue>
                     <xsl:choose>
                        <xsl:when test="contains(current(), &apos;0x&apos;)">
                           <Hex>
                              <xsl:value-of select="normalize-space (substring-before(substring-after(current(), &apos;x&apos;), &apos;:&apos;))"/>
                           </Hex>
                        </xsl:when>
                        <xsl:otherwise>
                           <Dec>
                              <xsl:value-of select="normalize-space (substring-before(current(), &apos;:&apos;))"/>
                           </Dec>
                        </xsl:otherwise>
                     </xsl:choose>
                  </ConstantValue>
                  <ConstantShortName>
                     <xsl:value-of select="normalize-space (substring-after(substring-before(current(), &apos;,&apos;), &apos;:&apos;))"/>
                  </ConstantShortName>
                  <ConstantDescription>
                     <xsl:value-of select="substring-after(current(), &apos;,&apos;)"/>
                  </ConstantDescription>
               </Constant>
            </xsl:for-each>
         </xsl:when>
         <xsl:otherwise>
            <xsl:for-each select="listitem/para">
               <List>
                  <Item>
                     <xsl:choose>
                        <xsl:when test="contains(current(), &apos;0x&apos;) and contains(current(), &apos;:&apos;)">
                           <Hex>
                              <xsl:value-of select="normalize-space (substring-before(substring-after(current(), &apos;x&apos;), &apos;:&apos;))"/>
                           </Hex>: <xsl:value-of select="substring-after(current(), &apos;: &apos;)"/>
                        </xsl:when>
                        <xsl:otherwise>
                           <xsl:value-of select="."/>
                        </xsl:otherwise>
                     </xsl:choose>
                  </Item>
               </List>
            </xsl:for-each>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
   <!-- default action: delete -->
   <xsl:template match="itemizedlist">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="listitem">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="parameterlist">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="parameteritem">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="parameternamelist">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="parameterdescription">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="programlisting">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="codeline">
	</xsl:template>
   <!-- default action: delete -->
   <xsl:template match="simplesect">
	</xsl:template>
   <!-- print code -->
   <xsl:template match="codeline" mode="generate">
      <Code>
         <xsl:apply-templates/>
      </Code>
   </xsl:template>
   <!-- print description text -->
   <xsl:template match="simplesect" mode="generate">
      <DescriptionText>
         <xsl:for-each select="para">
            <xsl:value-of select="."/>
         </xsl:for-each>
      </DescriptionText>
   </xsl:template>
   <xsl:template match="simplesect" mode="attention">
      <Attention/>
      <xsl:for-each select="para">
         <Paragraph>
            <xsl:apply-templates select="."/>
         </Paragraph>
         <xsl:apply-templates select="itemizedlist" mode="generate"/>
         <xsl:apply-templates select="orderedlist" mode="generate"/>
      </xsl:for-each>
      <Paragraph/>
   </xsl:template>
   <xsl:template match="simplesect" mode="note">
      <xsl:for-each select="para">
         <Comments>
            <Comment>
               <xsl:value-of select="."/>
            </Comment>
         </Comments>
      </xsl:for-each>
      <xsl:apply-templates select="itemizedlist" mode="generate"/>
      <xsl:apply-templates select="orderedlist" mode="generate"/>
   </xsl:template>
   <!-- print brief description text, used inside of tables -->
   <xsl:template match="briefdescription">
      <DescriptionText>
         <xsl:value-of select="normalize-space(para)"/>
      </DescriptionText>
   </xsl:template>
   <!-- print description text, used inside of tables -->
   <xsl:template match="detaileddescription">
      <!-- get first all text part, but not disturbing listings etc (see default action for codeline) -->
      <xsl:variable name="DESCRIPTION_LONG">
         <xsl:apply-templates select="para"/>
      </xsl:variable>
      <xsl:variable name="DESCRIPTION_NORM" select="normalize-space($DESCRIPTION_LONG)"/>
      <xsl:if test="string-length($DESCRIPTION_NORM)">
         <DescriptionText>
            <!--				&lt;xsl:value-of select=&quot;$DESCRIPTION_NORM&quot;/&gt; -->
            <xsl:apply-templates select="para"/>
         </DescriptionText>
         <xsl:for-each select="para/simplesect[@kind=&apos;remark&apos; or @kind=&apos;note&apos; or @kind=&apos;attention&apos;]/para">
            <Comments>
               <Comment>
                  <xsl:apply-templates select="."/>
               </Comment>
            </Comments>
         </xsl:for-each>
      </xsl:if>
      <!-- now print all simple list&apos;s -->
      <xsl:if test="para/itemizedlist">
         <xsl:apply-templates select="para/itemizedlist" mode="valuelist"/>
      </xsl:if>
   </xsl:template>
   <!-- generate example code -->
   <xsl:template name="generate_code_example">
      <xsl:if test="descendant::*[self::programlisting/codeline]">
         <xsl:element name="SW_Block">
            <xsl:attribute name="Role">Example</xsl:attribute>
            <xsl:element name="Title"/>
            <xsl:for-each select="descendant::*[self::programlisting]/codeline">
               <xsl:apply-templates select="." mode="generate"/>
            </xsl:for-each>
         </xsl:element>
      </xsl:if>
   </xsl:template>
   <!-- generate remarks -->
   <xsl:template name="generate_code_remark">
      <xsl:for-each select="detaileddescription/para/simplesect[@kind=&apos;remark&apos;]">
         <xsl:choose>
            <xsl:when test="para/itemizedlist">
               <xsl:element name="SW_Block">
                  <xsl:attribute name="Role">other</xsl:attribute>
                  <xsl:element name="Title">Compliance</xsl:element>
                  <xsl:apply-templates select="para/itemizedlist" mode="generate"/>
               </xsl:element>
            </xsl:when>
            <xsl:otherwise>
               <xsl:element name="SW_Block">
                  <xsl:attribute name="Role">Remarks</xsl:attribute>
                  <xsl:element name="Title"/>
                  <xsl:element name="Paragraph">
                     <xsl:value-of select="para"/>
                  </xsl:element>
               </xsl:element>
            </xsl:otherwise>
         </xsl:choose>
      </xsl:for-each>
   </xsl:template>
   <!-- print a space -->
   <xsl:template match="sp">
      <xsl:text> </xsl:text>
   </xsl:template>
   <!-- generate crossreference of current node-->
   <xsl:template name="generate_ref">
      <xsl:param name="VALUE"/>
      <xsl:choose>
         <xsl:when test="//compounddef[compoundname=substring-before($VALUE, &apos;*&apos;)]">
            <!-- a defined type -->
            <xsl:element name="CrossRef">
               <!--
					&lt;xsl:attribute name=&quot;Idref&quot;&gt;&lt;xsl:value-of select=&quot;//compounddef[compoundname=substring-before($VALUE, &apos;*&apos;)]/@id&quot;/&gt;&lt;/xsl:attribute&gt;
-->
               <xsl:attribute name="Idref"><xsl:value-of select="substring-before($VALUE, &apos;*&apos;)"/></xsl:attribute>
               <xsl:attribute name="type">Text</xsl:attribute>
            </xsl:element>
            <xsl:if test="contains($VALUE, &apos;*&apos;)">
               <xsl:text>*</xsl:text>
            </xsl:if>
         </xsl:when>
         <xsl:otherwise>
            <!-- nothing found, just print text -->
            <xsl:value-of select="$VALUE"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
   <!-- print crossreference -->
   <xsl:template match="ref">
      <xsl:variable name="VAR" select="@refid"/>
      <xsl:variable name="X" select="//*[@id=$VAR]"/>
      <xsl:variable name="VAR1">
         <xsl:value-of select="$X/name"/>
         <xsl:value-of select="$X/compoundname"/>
      </xsl:variable>
      <xsl:choose>
         <xsl:when test="normalize-space($VAR1) != &apos;&apos; ">
            <xsl:value-of select="normalize-space($VAR1)"/>
            <!--
				&lt;CrossRef&gt;
					&lt;xsl:attribute name=&quot;Idref&quot;&gt;&lt;xsl:value-of select=&quot;normalize-space($VAR1)&quot;/&gt;&lt;/xsl:attribute&gt;
					&lt;xsl:attribute name=&quot;type&quot;&gt;Text&lt;/xsl:attribute&gt;
				&lt;/CrossRef&gt;
				-->
         </xsl:when>
         <xsl:otherwise>
            <CrossRef>
               <xsl:attribute name="Idref"><xsl:value-of select="@refid"/></xsl:attribute>
               <xsl:attribute name="type">Text</xsl:attribute>
            </CrossRef>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
   <!-- print possible return values within table -->
   <xsl:template match="para" mode="return">
      <DescriptionText>
         <xsl:apply-templates select="."/>
      </DescriptionText>
      <xsl:apply-templates select="itemizedlist" mode="valuelist"/>
   </xsl:template>
   <xsl:template match="type">
      <xsl:apply-templates select="." mode="generate_ref"/>
   </xsl:template>
   <xsl:template match="ref" mode="generate_ref">
      <xsl:variable name="VAR" select="@refid"/>
      <xsl:variable name="X" select="//*[@id=$VAR]"/>
      <xsl:variable name="VAR1">
         <xsl:value-of select="$X/name"/>
         <xsl:value-of select="$X/compoundname"/>
      </xsl:variable>
      <xsl:choose>
         <xsl:when test="$VAR1 != &apos;&apos; ">
            <xsl:element name="CrossRef">
               <xsl:attribute name="Idref"><xsl:value-of select="$VAR1"/></xsl:attribute>
               <xsl:attribute name="type">Text</xsl:attribute>
            </xsl:element>
         </xsl:when>
         <xsl:otherwise>
            <xsl:element name="CrossRef">
               <xsl:attribute name="Idref"><xsl:value-of select="@refid"/></xsl:attribute>
               <xsl:attribute name="type">Text</xsl:attribute>
            </xsl:element>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
   <xsl:template name="generate_return_table">
      <xsl:if test="count(detaileddescription/para/simplesect[@kind=&apos;return&apos;]/para)">
         <xsl:variable name="DATATYPENORM" select="normalize-space(type)"/>
         <xsl:if test="not(contains($DATATYPENORM, &apos;void&apos;)) or (contains($DATATYPENORM, &apos;void&apos;) and contains($DATATYPENORM, &apos;*&apos;))">
            <xsl:element name="SW_Block">
               <xsl:attribute name="Role">ReturnValues</xsl:attribute>
               <xsl:element name="Title"/>
               <xsl:element name="SW_Table">
                  <xsl:element name="ReturnValuesTable">
                     <xsl:call-template name="generate_rv_head"/>
                     <xsl:element name="RV_Body">
                        <xsl:element name="RV_Row">
                           <xsl:element name="DataType">
                              <xsl:choose>
                                 <xsl:when test="contains(normalize-space(type),&apos;(*&apos;)">
                                    <xsl:value-of select="substring-before(normalize-space(type),&apos;(*&apos;)"/>
                                 </xsl:when>
                                 <xsl:otherwise>
                                    <xsl:apply-templates select="type"/>
                                 </xsl:otherwise>
                              </xsl:choose>
                           </xsl:element>
                           <xsl:element name="SW_Description">
                              <xsl:apply-templates select="detaileddescription/para/simplesect[@kind=&apos;return&apos;]/para" mode="return"/>
                           </xsl:element>
                        </xsl:element>
                     </xsl:element>
                  </xsl:element>
               </xsl:element>
            </xsl:element>
         </xsl:if>
      </xsl:if>
   </xsl:template>
   <xsl:template name="generate_dt_table">
      <xsl:element name="SW_Block">
         <xsl:attribute name="Role">Parameters</xsl:attribute>
         <xsl:element name="Title"/>
         <xsl:element name="SW_Table">
            <xsl:element name="DataTypeTable">
               <xsl:call-template name="generate_dt_head"/>
               <xsl:element name="DT_Body">
                  <xsl:for-each select="sectiondef/memberdef">
                     <xsl:element name="DT_Row">
                        <xsl:element name="DataType">
                           <xsl:apply-templates select="./type"/>
                        </xsl:element>
                        <xsl:element name="ParamName">
                           <!--&lt;xsl:attribute name=&quot;Id&quot;&gt;&lt;xsl:value-of select=&quot;@id&quot;/&gt;&lt;/xsl:attribute&gt;-->
                           <xsl:value-of select="normalize-space(./name)"/>
                           <xsl:value-of select="normalize-space(./argsstring)"/>
                        </xsl:element>
                        <xsl:element name="SW_Description">
                           <xsl:apply-templates select="briefdescription"/>
                           <xsl:apply-templates select="detaileddescription"/>
                        </xsl:element>
                     </xsl:element>
                  </xsl:for-each>
               </xsl:element>
            </xsl:element>
         </xsl:element>
      </xsl:element>
   </xsl:template>
</xsl:stylesheet>
