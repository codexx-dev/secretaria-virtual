
import { twMerge } from "tailwind-merge";

const sizeMap = {
  'A4-width': '210mm',
  'A4-height': '297mm',
}

function PaperPage({
  children, className, size = 'A4', orientation = 'portrait',
  marginLeft = '15mm', marginRight = '15mm', marginBotton = '15mm', marginTop = '15mm'
}) {
  const paperWidth =  (orientation === 'landscape')? 
    sizeMap[size + '-height'] : sizeMap['A4-width'];
    
  const paperHeight = (orientation === 'landscape')? 
    sizeMap[size + '-width']  : sizeMap['A4-height'];

  const style = {
    width: paperWidth,
    height: paperHeight,
    paddingLeft: marginLeft, 
    paddingRight: marginRight,
    paddingBottom: marginBotton,
    paddingTop: marginTop
  };

  const printStyle = `@page {
    size: ${size} ${orientation};
  }`;

  return (
    <>
      <div
        className={twMerge('paper-page bg-white shrink-0 grow-0', className)}
        style={style}
      >
        {children}
      </div>
      <style>
        {printStyle}
      </style>
    </>

  )
}

export default PaperPage;