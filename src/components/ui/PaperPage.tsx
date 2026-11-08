import { children, JSX } from "solid-js";

type PaperPageProps = {
  orientation: "vertical" | "horizontal"
  size: "a4" | "a3" | "letter" | "oficio"
  children?: JSX.Element;
}

export function PaperPage(props: PaperPageProps){

  const getWidth = () => {
    if(props.orientation === "horizontal"){
      switch(props.size){
        case "a4":
          return "297mm";
        case "a3":
          return "420mm";
        case "letter":
          return "279mm";
        case "oficio":
          return "356mm";
      }
    }else{
      switch(props.size){
        case "a4":
          return "210mm";
        case "a3":
          return "297mm";
        case "letter":
          return "216mm";
        case "oficio":
          return "216mm";
      }
    }
  }

  const getHeight = () => {
    if(props.orientation === "vertical"){
      switch(props.size){
        case "a4":
          return "297mm";
        case "a3":
          return "420mm";
        case "letter":
          return "279mm";
        case "oficio":
          return "356mm";
      } 
    }else{
      switch(props.size){
        case "a4":
          return "210mm";
        case "a3":
          return "297mm";
        case "letter":
          return "216mm";
        case "oficio":
          return "216mm";
      }
    }
  }

  return(
    <div class="bg-white" style={{width: getWidth(), height: getHeight()}}>
      {props.children}
    </div>
  );
}