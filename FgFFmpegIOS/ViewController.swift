//
//  ViewController.swift
//  FgFFmpegIOS
//
//  Created by redos on 2020/2/11.
//  Copyright © 2020 redos. All rights reserved.
//

import UIKit
import FgFFmpeg

class ViewController: UIViewController, FgFFmpegProgressCallback {
    func onProgress(_ taskId: Int, status: Int32, frames: Int, totalFrames: Int) {
        print("frames: %d", frames)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }

    @IBAction func onStartTranscode(_ sender: Any) {
        let filePath = Bundle.main.path(forResource: "demo", ofType: "mp4")
        let info = FgAVUtils.getFileInfo(filePath!)
        print(info)
        
        FgAVUtils.ffmpegMain(1,
                             argv: ["ffmpeg", "-i", filePath, "-y",
                             "-c:v", "mpeg4", "-c:a", "aac", "-strict",
                             "experimental", "-b:a", "192k", "-shortest",
                             "/Users/redos/z/Temp/demo_output.mp4"],
                             callback: self)
    }
    
}

