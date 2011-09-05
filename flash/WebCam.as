//TODO: Make all necessary dummy checks
package{
  import flash.events.*;
	import flash.display.Sprite;
	import flash.media.Camera;
	import flash.media.Video;
  import flash.display.BitmapData;
  import flash.display.Bitmap;
	import flash.geom.Rectangle;
  import flash.geom.Point;
  import flash.utils.ByteArray;

  import cmodule.camlib.CLibInit;

	public class WebCam extends Sprite
	{	
		private var camera:Camera;
		private var video:Video;
    
    private var frameCapture:BitmapData; 
    private var frameDraw:BitmapData;
    private var bmp:Bitmap;

    private var _dataPosition:uint;  // C memory bytes offset
    private var cByteData:ByteArray; // C memory bytes pointer
    private var cDataWidth:uint;
    private var cDataHeight:uint;
    private var cDataChannels:uint;
	
    //C library loader
    private var lib:Object;

    //WebCam constructor and entry point
		public function WebCam():void {
      trace("New Session started------------------------------------------------------------------------");
      //initialize C library
      setupCLib();
      //initialize Camera
			setupCamera();

      //Add exit event listener
      stage.addEventListener(KeyboardEvent.KEY_DOWN,keyboardEventKeyDownHandler);
    }

    private function setupCLib():void
    {
      var loader:CLibInit=new CLibInit();
      lib=loader.init();
      var ns:Namespace = new Namespace("cmodule.camlib");
      //get C byte data pointer (that is, C RAM chunk)
      cByteData=(ns::gstate).ds; 
      trace("Initilaized C library");
      trace("Total RAM size: "+cByteData.length);
      trace("Data start offset: "+cByteData.position);
      trace("Bytes available: "+cByteData.bytesAvailable);
    }
		
    private function setupCamera():void
    {
      camera = Camera.getCamera();
      //initialized camera
      if (camera!=null){
        camera.setMode(640,480,20,true);

        //make a new video obj
        video=new Video(camera.width,camera.height);
        video.attachCamera(camera);

        //make a framedata for a current frame and for draw frame
        frameCapture=new BitmapData(video.width, video.height);
        frameDraw=new BitmapData(video.width,video.height);

        //acquire first frame to make a correct pixel storage
        frameCapture.draw(video);
        var bounds:Rectangle=new Rectangle(0,0,frameCapture.width,frameCapture.height);
        var pixels:ByteArray = frameCapture.getPixels(bounds);
        //get pointer to C byte data
        trace("Allocating "+pixels.length+" bytes in C memory");
        _dataPosition=lib.initByteArray(pixels.length);
        trace("Allocated at "+_dataPosition);
        
        trace("Transfering frame parameters to C libraries");
        //set frame properties for C memory libraries
        //lib.setFrameParams(frameCapture.width,frameCapture.height,pixels.length/(frameCapture.width*frameCapture.height));
        //create a drawing bitmap
        bmp =new Bitmap(frameDraw);

        //make an enter frame listener
        //addEventListener(Event.ENTER_FRAME,enterFrameHandler);

        //Add bmp to show 
        addChild(bmp);
      } else {
        trace("Camera not ready");
      }
    }

    private function enterFrameHandler(event:Event):void
    {
      if (camera!=null)
      {
        trace("Get a frame from camera");
        frameCapture.draw(video);

        trace("Get pixel data from frame");
        var bounds:Rectangle=new Rectangle(0,0,frameCapture.width,frameCapture.height);
        var pixels:ByteArray = frameCapture.getPixels(bounds);
        pixels.position=0;
        trace("Pixels size in bytes: "+pixels.length);
        //frameDraw.setPixels(bounds,pixels);

        //Here we can make it faster?

        //copy bytes from pixels to C memory
        cByteData.writeBytes(pixels,0,pixels.length);
        trace("Wrote data to C memory at "+_dataPosition+" ("+cByteData.bytesAvailable+" more bytes avaliable)");
        trace("Wrote "+(cByteData.position-_dataPosition)+" bytes");
        cByteData.position=_dataPosition;

        trace("Here be C++ OpenCV magic");
        lib.testCV();
        trace("Magic OK");

        trace("Set draw frame from C memory");
        pixels.writeBytes(cByteData,_dataPosition,cByteData.length-_dataPosition);
        pixels.position=0;
        frameDraw.setPixels(bounds,pixels);
        cByteData.position=_dataPosition;//JIC
      }
    }

    private function keyboardEventKeyDownHandler(e:KeyboardEvent):void
    {
      trace(String.fromCharCode(e.charCode)+" Key code:"+e.keyCode);
      if (e.keyCode==27){//Esc pressed
        trace("Unreg and remove handlers");
        removeEventListener(Event.ENTER_FRAME,enterFrameHandler);
        removeEventListener(KeyboardEvent.KEY_DOWN,keyboardEventKeyDownHandler);
        trace("Dealloc C memory (disabled due to dealloc errors - fix it)");
        cByteData=null;
        //lib.freeByteArray();
        trace("Unshow bmp and release it");
        removeChild(bmp);
        bmp=null;
        trace("Release frame data");
        frameDraw.dispose();
        frameCapture.dispose();
        trace("Release video");
        video=null;
        trace("Release camera");
        camera=null;
      } else {
        if (e.keyCode==67){//c
          trace("Testing heap allocation");
          trace("RAM offset: "+cByteData.position);
          lib.testCV();
          trace("new RAM offset: "+cByteData.position);
        } else {
          if (e.keyCode==65){//a
            trace("Allocating in heap at "+_dataPosition);
            _dataPosition=lib.initByteArray(320*240);
          }
        }
      } 
    }
  }
}
