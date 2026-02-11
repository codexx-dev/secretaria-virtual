import { twMerge } from "tailwind-merge";

function ImgIcon({className, width, height, src, text}){
  return (
    <span className={twMerge('flex gap-1 items-center', className)}>
      <div className="min-w-max">
        <img width={width} height={height} src={src}/>
      </div>
      {text}
    </span>
  );
}

export default ImgIcon;