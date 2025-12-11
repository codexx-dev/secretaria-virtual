
import "./PaperPage.css";

function PaperPage({
  children, className, 
  size = 'a4', orientation = 'portrait',
  marginLeft = '15mm', marginRight = '15mm', marginBotton = '10mm', marginTop = '5mm'
}) {

  return (
    <div
      className={`paper-page ${size} ${orientation} ${className}`}
      style={{paddingLeft: marginLeft, paddingRight: marginRight, paddingBottom: marginBotton, paddingTop: marginTop}}
    >
      {children}
    </div>
  )
}

export default PaperPage;