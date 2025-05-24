import cv2 
import matplotlib.pyplot as plt
import numpy as np
from cvzone.SerialModule import SerialObject
bluetooth=SerialObject("COM13") 
a=0 
def classname_to_idx(name_list,label_dict):
    idx_list = []
    for name in name_list:
        if name in label_dict.values():
            idx_list.append(list(label_dict.values()).index(name))
    return idx_list

def idx_to_classname(idx_list,label_dict):
    classname_list = []
    for idx in idx_list:
        if idx in label_dict.keys():
            classname_list.append(label_dict[idx])  
    return classname_list


## load tiny yolov4 model
config_path = "yolov7-tiny.cfg"
weights_path = "yolov7-tiny.weights"

net = cv2.dnn.readNetFromDarknet(config_path,weights_path)

labels = []
with open('coco.names','r') as f:
    labels = [line.strip() for line in f.readlines()]
    
label_dict = dict(zip(list(range(len(labels))),labels))
    
FONT = cv2.FONT_HERSHEY_SIMPLEX
font_scale = 0.5
thickness = 1
colors = np.random.randint(0, 255, size=(len(labels), 3), dtype="uint8")

CONFIDENCE = 0.5
SCORE_THRESHOLD = 0.5
IOU_THRESHOLD = 0.5



cap = cv2.VideoCapture("http://192.168.184.95:81/stream")
while True:
    ret,frame = cap.read()
    if not ret:
        break
    
    image = cv2.resize(frame,(1080,720))
    h, w = image.shape[:2]
    blob = cv2.dnn.blobFromImage(image,1/255.0,(416,416),swapRB=True,crop=False)
    net.setInput(blob)
    last_layer = net.getUnconnectedOutLayersNames()
    layer_outputs = net.forward(last_layer)

    boxes = []
    confidences = []
    class_ids = []
    allowed_classes = list(range(len(labels)))

    for output in layer_outputs:
        for detection in output:
            score = detection[5:]
            class_id = np.argmax(score)
            confidence = score[class_id]
            if confidence > CONFIDENCE:
                box = detection[:4] * np.array([w, h, w, h])
                (centerX, centerY, width, height) = box.astype("int")
                x = int(centerX - (width / 2))
                y = int(centerY - (height / 2))
                if class_id in allowed_classes:
                    boxes.append([x, y, int(width), int(height)])
                    confidences.append(float(confidence))
                    class_ids.append(class_id)

    indexes = cv2.dnn.NMSBoxes(boxes,confidences,SCORE_THRESHOLD, IOU_THRESHOLD)
    if len(indexes) > 0:
        for i in indexes.flatten():
            x, y = boxes[i][0], boxes[i][1]
            w, h = boxes[i][2], boxes[i][3]
            color = [int(c) for c in colors[class_ids[i]]]
            cv2.rectangle(image, (x, y), (x + w, y + h), color=color, thickness=thickness)
            text = f"{labels[class_ids[i]]}: {confidences[i]:.2f}"
            (text_width, text_height) = cv2.getTextSize(text, FONT, fontScale=font_scale, thickness=thickness)[0]
            text_offset_x = x
            text_offset_y = y - 5
            box_coords = ((text_offset_x, text_offset_y), (text_offset_x + text_width + 2, text_offset_y - text_height))
            overlay = image.copy()
            cv2.rectangle(overlay, box_coords[0], box_coords[1], color=color, thickness=cv2.FILLED)
            image = cv2.addWeighted(overlay, 0.6, image, 0.4, 0)
            cv2.putText(image, text, (x, y - 5), FONT,fontScale=font_scale, color=(0, 0, 0), thickness=thickness)
            det_label = labels[class_ids[i]]
            print(labels[class_ids[i]])
            if (det_label=='person'):
                a=1
                print(a)
            else:
                a=0
                print(a)
    cv2.imshow("FRAME",image)
    bluetooth.sendData([a]) 
    key = cv2.waitKey(1)
    if key==27:
        break
    
cap.release()
cv2.destroyAllWindows()