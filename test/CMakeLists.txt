add_subdirectory(data)

# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)
#add_grail_executable(SRC ESRIMapDemo.cc LIBS grail)
# add_grail_executable(SRC JoeyDemo.cc LIBS grail)
# add_grail_executable(SRC MapDemo.cc LIBS grail)
# add_grail_executable(SRC StockDemo.cc LIBS grail)
# add_grail_executable(SRC Globe.cc LIBS grail)
# add_grail_executable(SRC ../src/xp/decompressBlockLoader.cc LIBS grail lzma)
# add_grail_executable(SRC simpleDemo.cc LIBS grail)
# add_grail_executable(SRC SolarSystem.cc LIBS grail)
# add_grail_executable(SRC testAxis.cc LIBS grail)
# add_grail_executable(SRC testBarChart.cc LIBS grail)
# add_grail_executable(SRC testBoxChart.cc LIBS grail)
# add_grail_executable(SRC testCandlestickChart.cc LIBS grail)
# add_grail_executable(SRC testDisplayBook.cc LIBS grail)
# add_grail_executable(SRC testDrawBlockMap.cc LIBS grail)
# add_grail_executable(SRC testESRIPolygon.cc LIBS grail)
# add_grail_executable(SRC testGrid.cc LIBS grail)
# add_grail_executable(SRC testImage.cc LIBS grail)
# add_grail_executable(SRC testLineGraph.cc LIBS grail)
# add_grail_executable(SRC testPolyLines.cc LIBS grail)
# add_grail_executable(SRC testStyledMultishape.cc LIBS grail)
# add_grail_executable(SRC testText3.cc LIBS grail)
add_grail_executable(SRC testWidgets.cc LIBS grail)
# add_grail_executable(SRC testDrawing3D.cc LIBS grail)
# add_grail_executable(SRC ../src/csp/GenericXDLClient.cc LIBS grail)
# add_grail_executable(SRC testPopupMenu.cc LIBS grail)
add_grail_executable(SRC CurveTest.cc LIBS grail)
add_grail_executable(SRC HelixTest.cc LIBS grail)
add_grail_executable(SRC BezierTest.cc LIBS grail)
add_grail_executable(SRC Vec3DTest.cc LIBS grail)
add_grail_executable(SRC CircleTest.cc LIBS grail)
add_grail_executable(SRC ExtrusionTest.cc LIBS grail)
add_grail_executable(SRC FacetTest.cc LIBS grail)
add_grail_executable(SRC LineTest.cc LIBS grail)
add_grail_executable(SRC LinkedPathTest.cc LIBS grail)
#add_grail_executable(SRC CurveTest.cc LIBS grail)
add_grail_executable(SRC testCylinder.cc LIBS grail)
add_grail_executable(SRC testSphere.cc LIBS grail)
add_grail_executable(SRC testRectangularPrism.cc LIBS grail)
#add_grail_executable(BINNAME testSolar SRC DrawNASAEphemerisSolarSystem2d.cc LIBS grail)
add_grail_executable(SRC testTriangle.cc LIBS grail)
add_grail_executable(SRC testRectangle.cc LIBS grail)
add_grail_executable(SRC testCircle.cc LIBS grail)
add_grail_executable(SRC testGear.cc LIBS grail)



if(${MPV_FOUND})
  # add_grail_executable(SRC testAudio.cc LIBS grail)
  # add_grail_executable(SRC testVideo.cc LIBS grail)
endif()
# setting up a new executable, only needs the two lines
